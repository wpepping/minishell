/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/24 01:25:53 by phartman         ###   ########.fr       */
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

int	get_args(t_list *tokens, t_parse_node *node)
{
	int	i;
	int	argc;
	t_list	*tmp;

	tmp = tokens;

	argc = 0;
	i = 0;
	while(tokens->next && tokens->content.type == WORD)
	{
		argc++;
		tokens = tokens->next;
	}
	tokens = tmp;
	node->argv = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		node->argv[i] = strdup(tokens->content.value);
		if (!node->argv[i])
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		i++;
		tokens->next;
	}
	node->argv[i] = NULL;
	return (argc);
}

int	handle_redirects(t_list *tokens, t_parse_node *node)
{
	if (tokens->content.type == APPEND)
	{
		ft_lstadd_back(&node->output_dest, ft_lstnew(ft_strdup(tokens->next->content.value)));
		node->append = true;
	}
	else if (tokens->content.type == REDIRECT_OUT)
		ft_lstadd_back(&node->output_dest, ft_lstnew(ft_strdup(tokens->next->content.value)));
	else if (tokens->content.type == REDIRECT_IN)
		ft_lstadd_back(&node->input_src, ft_lstnew(ft_strdup(tokens->next->content.value)));
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

	// else if(!open_single_quote)
	// {
	// 	first_quote = ft_strchr(token, '\"');
	// 	last_quote = ft_strrchr(token, '\"');
	// 	if(first_quote && first_quote == last_quote)
	// 		open_double_quote = !open_double_quote;
	// }
	// else if(!open_double_quote)
	// {
	// 	first_quote = ft_strchr(token, '\'');
	// 	last_quote = ft_strrchr(token, '\'');
	// 	if(first_quote && first_quote == last_quote)
	// 		open_single_quote = !open_single_quote;
	// }
	return (open_double_quote || open_single_quote);

}

bool	append_token(t_list **token_list, t_token_type type, char **cmd, int len)
{
	t_token	*token;

	
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		printf("Error: malloc failed\n");
		return(false);
	}
	new_token->value = ft_strdup(cmd, 0 , len);
	if (!new_token->value)
	{
		printf("Error: malloc failed\n");
		return(false);
	}
	new_token->type = type;
	ft_lstadd_back(token_list, ft_lstnew(new_token));
	while (len > 0)
	{
		(*cmd)++;
		len--;
	}

	return (true);
}


t_list	*tokenize(char *cmd)
{
	t_list	*token_list;
	int		i;
	int		j;

	token_list = NULL;
	i = 0;
	while (*cmd)
	{
		if (*cmd == ' ')
			cmd++;
		else if (!(ft_strncmp(cmd, "&&", 2)) || !(ft_strncmp(cmd, "<", 1))
			|| !(ft_strncmp(cmd, ">", 1)) || !(ft_strncmp(cmd, "(", 1))
			|| !(ft_strncmp(cmd, ")", 1) || !(ft_strncmp(cmd, "|", 1))))
		{
			if(ft_strncmp(cmd, "&&", 2) == 0)
				append_token(&token_list, AND, cmd, 2);
			else if(ft_strncmp(cmd, "|", 2) == 0)
				append_token(&token_list, OR, cmd, 2);
			else if(ft_strncmp(cmd, "<<", 2) == 0)
				append_token(&token_list, HEREDOC, cmd, 2);
			else if(ft_strncmp(cmd, ">>", 2) == 0)
				append_token(&token_list, APPEND, cmd, 2);
			else if(ft_strncmp(cmd, ">", 1) == 0)
				append_token(&token_list, REDIRECT_OUT, cmd, 1);
			else if(ft_strncmp(cmd, "<", 1) == 0)
				append_token(&token_list, REDIRECT_IN, cmd, 1);
			else if(ft_strncmp(cmd, "(", 1) == 0)
				append_token(&token_list, OPEN_PAREN, cmd, 1);
			else if(ft_strncmp(cmd, ")", 1) == 0)
				append_token(&token_list, CLOSE_PAREN, cmd, 1);
			else if(ft_strncmp(cmd, "|", 1) == 0)
				append_token(&token_list, PIPE, cmd, 1);
		}
		else
		{
			while(cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '<' && cmd[i] != '>'
				&& cmd[i] != '(' && cmd[i] != ')' && cmd[i] != '&' && cmd[i] != '\t')
			{

				if(in_quotes(cmd[i]))
				{
					while(cmd[i] && in_quotes(cmd[i]))
						i++;
				}
				else
					i++;
			}
			append_token(&token_list, WORD, cmd, i);
			i=0;
		}
	return (token_list);
	}
}


void	parse(t_data *data, char *cmd)
{
	t_parse_node	*node;
	t_list			*tokens;
	t_list			*head;
	char			*quote_arg;
	quote_arg = NULL;
	i= 0;

	if (cmd == NULL || ft_strncmp(cmd, "exit", 5) == 0)
	{
		data->exit = 1;
		return ;
	}
	tokens = tokenize(cmd);
	head = tokens;
	//tokens = ft_split(cmd, ' ');
	node = create_parse_node();
	
	while (tokens->next)
	{
		
		if(tokens->content.type == PIPE)
		{
			print_argv(node);
			ft_lstadd_back(&data->node_list, ft_lstnew(node));
			node = create_parse_node();
			tokens = tokens->next;
			continue;
			
		}
		if (access(tokens->content.value, X_OK) == 0)
		{
			node->exec = ft_strdup(tokens->content.value);
			i += get_args(i, tokens, node);
			while(tokens->next && tokens->content.type == WORD && --i < 0)
				tokens = tokens->next;
		}
		else if (get_builtin_index(tokens->content.value) != -1)
		{
			node->is_builtin = true;
			node->exec = ft_strdup(tokens->content.value);
			while(tokens->next && tokens->content.type == WORD && --i < 0)
				tokens = tokens->next;
		}
		else
		{
			handle_redirects(tokens, i, node);
			tokens = tokens->next;
			tokens = tokens->next;
		}
			
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

