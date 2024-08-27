/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 02:07:25 by phartman         ###   ########.fr       */
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




bool in_quotes(char * token)
{

	static bool open_double_quote;
	static bool open_single_quote;
    //char *first_quote;
    //char *last_quote;

	if(ft_strncmp(token, "\"", 1) == 0 && !open_single_quote)
		open_double_quote = !open_double_quote;
	else if(ft_strncmp(token, "'", 1) == 0 && !open_double_quote)
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

	
	token = malloc(sizeof(t_token));
	if (!token)
	{
		printf("Error: malloc failed\n");
		return(false);
	}
	token->value = ft_substr(*cmd, 0 , len);
	
	if (!token->value)
	{
		printf("Error: malloc failed\n");
		return(false);
	}
	token->type = type;
	printf("\ntoken value: %s\n", token->value);
	printf("\ntoken type: %i\n", token->type);
	ft_lstadd_back(token_list, ft_lstnew(token));
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
	//int		j;

	token_list = NULL;
	i = 0;
	while (*cmd)
	{
		if (*cmd == ' ')
			cmd++;
		else if (!(ft_strncmp(cmd, "&&", 2)) || !(ft_strncmp(cmd, "|", 1))
        || !(ft_strncmp(cmd, ">", 1)) || !(ft_strncmp(cmd, "(", 1))
        || !(ft_strncmp(cmd, ")", 1)) || !(ft_strncmp(cmd, "<", 1)))
		{
			if(ft_strncmp(cmd, "&&", 2) == 0)
				append_token(&token_list, AND, &cmd, 2);
			else if(ft_strncmp(cmd, "||", 2) == 0)
				append_token(&token_list, OR, &cmd, 2);
			else if(ft_strncmp(cmd, "<<", 2) == 0)
				append_token(&token_list, HEREDOC, &cmd, 2);
			else if(ft_strncmp(cmd, ">>", 2) == 0)
				append_token(&token_list, APPEND, &cmd, 2);
			else if(ft_strncmp(cmd, ">", 1) == 0)
				append_token(&token_list, REDIRECT_OUT, &cmd, 1);
			else if(ft_strncmp(cmd, "<", 1) == 0)
				append_token(&token_list, REDIRECT_IN, &cmd, 1);
			else if(ft_strncmp(cmd, "(", 1) == 0)
				append_token(&token_list, OPEN_PAREN, &cmd, 1);
			else if(ft_strncmp(cmd, ")", 1) == 0)
				append_token(&token_list, CLOSE_PAREN, &cmd, 1);
			else if(ft_strncmp(cmd, "|", 1) == 0)
				append_token(&token_list, PIPE, &cmd, 1);
		}
		else
		{
			while(cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '<' && cmd[i] != '>'
				&& cmd[i] != '(' && cmd[i] != ')' && cmd[i] != '&' && cmd[i] != '\t')
			{

				if(in_quotes(&cmd[i]))
				{
					i++;
					while(cmd[i] && in_quotes(&cmd[i]))
						i++;
					i++;
					
				}
				else
					i++;
			}
			append_token(&token_list, WORD, &cmd, i);
			
			i=0;
		}
	
	}
	return (token_list);
}

void	handle_redirects(t_list **tokens, t_parse_node *node)
{
	
	t_token *token;

	token = (t_token *)(*tokens)->content;
	if ((token->type == APPEND))
	{
		ft_lstadd_back(&node->output_dest, ft_lstnew(ft_strdup(((t_token *)(*tokens)->next->content)->value)));
		node->append = true;
	}
	else if (token->type == REDIRECT_OUT)
		ft_lstadd_back(&node->output_dest, ft_lstnew(ft_strdup(((t_token *)(*tokens)->next->content)->value)));
	else if (token->type == REDIRECT_IN)
		ft_lstadd_back(&node->input_src, ft_lstnew(ft_strdup(((t_token *)(*tokens)->next->content)->value)));
	//else if (token->type == HEREDOC)
		//node->heredoc = true;
	if((t_token *)(*tokens)->next->next != NULL)
		tokens = &(*tokens)->next->next;
}

int	get_args(t_list **tokens, t_parse_node *node)
{
	int	i;
	int	argc;
	t_list	*tmp;
	t_token *token;

	token = (t_token *)(*tokens)->content;
	tmp = *tokens;

	argc = 0;
	i = 0;
	while(tmp && ((t_token *)tmp->content)->type == WORD)
	{
		argc++;
		tmp = tmp->next;
	}
	node->argv = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		node->argv[i] = strdup(token->value);
		if (!node->argv[i])
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		i++;
		*tokens = (*tokens)->next;
		if(*tokens)
			token = (t_token *)(*tokens)->content;
	}
	node->argv[i] = NULL;
	return (argc);
}


void parse_args_and_redirects(t_list **tokens, t_parse_node *node)
{
	t_token *token;

	token = (t_token *)(*tokens)->content;
	if(token->type == WORD)
		get_args(tokens, node);
	else
		handle_redirects(tokens, node);
}

void parse_command(t_list *tokens, t_data *data)
{
	t_parse_node	*node;
	t_token			*token;

	if(tokens)
		token = (t_token *)tokens->content;

	node = create_parse_node();
	if(token->type == WORD && tokens)
	{
		
		if(get_builtin_index(token->value) != -1)
			node->is_builtin = true;
		else if(access(token->value, X_OK) == 0)
			node->exec = ft_strdup(token->value);
		parse_args_and_redirects(&tokens, node);
	}
	if(!tokens)
		node->is_last = true;
	ft_lstadd_back(&data->node_list, ft_lstnew(node));
	if(tokens)
		parse_pipe(&tokens, node, data);
	
	
}

void parse_pipe(t_list **tokens, t_parse_node *node , t_data *data)
{
	t_token *token;
	(void)node;
	token = (t_token *)(*tokens)->content;
	if(token->type == PIPE)
	{
		//print_argv(node);
		//ft_lstadd_back(&data->node_list, ft_lstnew(node));
		*tokens = (*tokens)->next;
		parse_command(*tokens, data);
	}
}


void	parse(t_data *data, char *cmd)
{
	//t_parse_node	*node;
	t_list			*tokens;
	//t_token			*token;
	//int i;
	//t_list			*head;
	//char			*quote_arg;
	//quote_arg = NULL;
	//i= 0;

	if (cmd == NULL || ft_strncmp(cmd, "exit", 5) == 0)
	{
		data->exit = 1;
		return ;
	}
	tokens = tokenize(cmd);
	parse_command(tokens, data);
	//head = tokens;
	//tokens = ft_split(cmd, ' ');
	// node = create_parse_node();
	
	// while (tokens->next)
	// {
	// 	token = (t_token *)(*tokens)->content;
	// 	if(token->type == PIPE)
	// 	{
	// 		print_argv(node);
	// 		ft_lstadd_back(&data->node_list, ft_lstnew(node));
	// 		node = create_parse_node();
	// 		tokens = tokens->next;
	// 		continue;
			
	// 	}
	// 	if (access(token->value, X_OK) == 0)
	// 	{
	// 		node->exec = ft_strdup(token->value);
	// 		i += get_args(tokens, node);
	// 		while(tokens->next && token->type == WORD && --i < 0)
	// 			tokens = tokens->next;
	// 	}
	// 	else if (get_builtin_index(token->value) != -1)
	// 	{
	// 		node->is_builtin = true;
	// 		node->exec = ft_strdup(token->value);
	// 		i += get_args(tokens, node);
	// 		while(tokens->next && (token->type == WORD && --i > 0))
	// 		{
	// 			tokens = tokens->next;
	// 			token = (t_token *)(*tokens)->content;
	// 		}
				
	// 	}
	// 	else
	// 	{
	// 		handle_redirects(tokens, node);
	// 		tokens = tokens->next;
			
	// 	}
	// }
	// node->is_last = true;
	// print_argv(node);
	// ft_lstadd_back(&data->node_list, ft_lstnew(node));
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

