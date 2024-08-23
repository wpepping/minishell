/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/23 20:06:14 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_builtin_index(char *token)
{
	int	i;
	const char *builtin_str[] = {
    "echo",
	"cd",
	"unset",
	"export",
	
	"pwd",
	"env",
	"exit"
	};
	
	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(token, builtin_str[i], ft_strlen(builtin_str[i])) == 0)
		{
			return (i);
			
		}
			
		i++;
	}
	return (-1);
}

int	get_args(int index, char **tokens, t_parse_node *node)
{
	int	i;
	int	argc;

	argc = 0;
	i = 0;
	while (tokens[index + argc] && strcmp(tokens[index + argc], "<") != 0
		&& strcmp(tokens[index + argc], ">") != 0 && strcmp(tokens[index + argc],
			"|") != 0)
		argc++;
	node->argv = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		node->argv[i] = strdup(tokens[index]);
		if (!node->argv[i])
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		i++;
		index++;
	}
	node->argv[i] = NULL;
	return (argc);
}

int	handle_redirects(char **tokens, int index, t_parse_node *node)
{

	int jump;

	jump = 0;
	if (ft_strncmp(tokens[index], ">>", 2) == 0)
		node->append = true;
	if (ft_strncmp(tokens[index], ">", 1) == 0)
	{
		ft_lstadd_back(&node->output_dest, ft_lstnew(ft_strdup(tokens[index
						+ 1])));
		jump= 2;
	}
	//else if (ft_strncmp(tokens[index], "<<", 2) == 0)
		//node->heredoc = true;
	else if (ft_strncmp(tokens[index], "<", 1) == 0)
	{
		ft_lstadd_back(&node->input_src, ft_lstnew(ft_strdup(tokens[index
						+ 1])));
		jump = 2;
	}
	return (jump);
}

bool in_quotes(char * token)
{

	static bool open_double_quote;
	static bool open_single_quote;
    char *first_quote;
    char *last_quote;

	if(ft_strncmp(token, "\"", 1) == 0 && !open_single_quote)
		open_double_quote = !open_double_quote;
	else if(ft_strncmp(token, "\'", 1) == 0 && !open_double_quote)
		open_single_quote = !open_single_quote;

	else if(!open_single_quote)
	{
		first_quote = ft_strchr(token, '\"');
		last_quote = ft_strrchr(token, '\"');
		if(first_quote && first_quote == last_quote)
			open_double_quote = !open_double_quote;
	}
	else if(!open_double_quote)
	{
		first_quote = ft_strchr(token, '\'');
		last_quote = ft_strrchr(token, '\'');
		if(first_quote && first_quote == last_quote)
			open_single_quote = !open_single_quote;
	}
	return (open_double_quote || open_single_quote);
	
	
}

void	parse(t_data *data, char *cmd)
{
	t_parse_node	*node;
	char			**tokens;
	int				i;
	char			*quote_arg;
	quote_arg = NULL;
	i= 0;

	if (cmd == NULL || ft_strncmp(cmd, "exit", 5) == 0)
	{
		data->exit = 1;
		return ;
	}
	tokens = ft_split(cmd, ' ');
	node = create_parse_node();
	
	while (tokens[i])
	{
		if((in_quotes(tokens[i]) == true))
		{
			if(quote_arg == NULL)
				quote_arg = ft_strdup(tokens[i]);
			else
			{
				char *temp = ft_strjoin(quote_arg, " ");
				free(quote_arg);
				quote_arg = ft_strjoin(temp, tokens[i]);
				free(temp);
			}
			i++;
			continue;
		}
		if(ft_strncmp(tokens[i], "|", 1) == 0)
		{
			print_argv(node);
			ft_lstadd_back(&data->node_list, ft_lstnew(node));
			node = create_parse_node();
			i++;
			continue;
			
		}
		if (access(tokens[i], X_OK) == 0)
		{
			node->exec = ft_strdup(tokens[i]);
			i += get_args(i, tokens, node);
		}
		else if (get_builtin_index(tokens[i]) != -1)
		{
			node->is_builtin = true;
			node->exec = ft_strdup(tokens[i]);
			i += get_args(i, tokens, node);
		}
		else
			i += handle_redirects(tokens, i, node);
	}
	node->is_last = true;
	print_argv(node);
	ft_lstadd_back(&data->node_list, ft_lstnew(node));
}

void print_argv(t_parse_node *node)
{
    if (node->argv == NULL)
    {
        printf("argv is NULL\n");
        return;
    }

    printf("argv contents:\n");
    for (int i = 0; node->argv[i] != NULL; i++)
    {
        printf("argv[%d]: %s\n", i, node->argv[i]);
    }
}

t_parse_node	*create_parse_node(void)
{
	t_parse_node	*node;

	node = malloc(sizeof(t_parse_node));
	if (!node)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	node->is_last = false;
	node->is_builtin = false;
	node->append = false;
	node->heredoc = false;
	node->exec = NULL;
	node->argv = NULL;
	node->output_dest = NULL;
	node->input_src = NULL;
	return (node);
}

