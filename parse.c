/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/22 16:59:12 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t			len;
	unsigned char	uc;

	uc = c;
	len = ft_strlen(s);
	if (uc == 0)
		return ((char *)&s[len]);
	while (len--)
	{
		if (s[len] == uc)
			return ((char *)&s[len]);
	}
	return (NULL);
}

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	unsigned char	uc;

	uc = c;
	i = 0;
	while (s[i])
	{
		if (s[i] == uc)
			return ((char *)&s[i]);
		i++;
	}
	if (uc == 0)
		return ((char *)&s[i]);
	return (NULL);
}

int	get_builtin_index(char *token)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(token, builtin_str[i], ft_strlen(builtin_str[i])) == 0)
			return (i);
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
		&& strcmp(tokens[index + argc], ">") != 0 && strcmp(tokens[index],
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

int in_quotes(char * token)
{

	static int open_quote;
    char *first_quote;
    char *last_quote;
	char quotes[] = {'\'', '\"'};
	int i;
	open_quote = 0;
	i = 0;
	while(i < 2)
	{	
		first_quote = ft_strchr(token, quotes[i]);
		last_quote = ft_strrchr(token, quotes[i]);
		if(!open_quote && first_quote && first_quote == last_quote)
			open_quote = 1;
		else if(open_quote && first_quote && first_quote == last_quote)
			open_quote = 0;
		else if(open_quote && first_quote && last_quote && first_quote != last_quote)
			open_quote = 1;
		i++;
	}
	return (open_quote);
	
}

void	parse(t_data *data, char *cmd)
{
	t_parse_node	*node;
	char			**tokens;
	int				i;

	i = 0;
	if (cmd == NULL || ft_strncmp(cmd, "exit", 5) == 0)
	{
		data->exit = 1;
		return ;
	}
	tokens = ft_split(cmd, ' ');
	node = create_parse_node();
	while (tokens[i])
	{
		if((in_quotes(tokens[i]) == 1))
		{
			i++;
			continue;
		}
		if(ft_strncmp(tokens[i], "|", 1) == 0)
		{
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
		i += handle_redirects(tokens, i, node);
	}
	ft_lstadd_back(&data->node_list, ft_lstnew(node));
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

