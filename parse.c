/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/02 17:24:46 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_builtin_index(char *token)
{
	int			i;
	const char	*builtin_str[] = {"echo", "cd", "unset", "export", "pwd", "env",
			"exit"};

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

bool is_valid_filename(t_token *token)
{
	if (token->value == NULL || ft_strlen(token->value) == 0 ||  (token->type != WORD && token->type != DOUBLE_QUOTE && token->type != SINGLE_QUOTE))
		return (false);
	
	return (true);
}

void parse_redirects(t_list **tokens, t_parse_node *node)
{
    t_token	*token;

    while (*tokens != NULL)
    {
        token = (t_token *)(*tokens)->content;
        if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN || token->type == APPEND)
        {
            if (!is_valid_filename((*tokens)->next->content))
            {
                printf("Error: no filename specified for redirection\n");
                exit(1);
            }
            *tokens = handle_redirects(*tokens, node);
        }
        else
        {
            break;
        }
    }
}


t_list *handle_redirects(t_list *tokens, t_parse_node *node)
{
    t_token *token;
    t_list *current;

    current = tokens;
    token = (t_token *)current->content;

    if (token->type == APPEND)
    {
        ft_lstadd_back(&node->output_dest,
            ft_lstnew(ft_strdup(((t_token *)current->next->content)->value)));
        node->append = true;
    }
    else if (token->type == REDIRECT_OUT)
    {
        ft_lstadd_back(&node->output_dest,
            ft_lstnew(ft_strdup(((t_token *)current->next->content)->value)));
    }
    else if (token->type == REDIRECT_IN)
    {
        ft_lstadd_back(&node->input_src,
            ft_lstnew(ft_strdup(((t_token *)current->next->content)->value)));
    }
    
    if (current->next != NULL)
        return current->next->next; // Return the list location after the filename
    else
        return current->next;
}
int	get_args(t_list **tokens, t_parse_node *node)
{
	int		i;
	int		argc;
	t_list	*tmp;
	t_token	*token;

	token = (t_token *)(*tokens)->content;
	tmp = *tokens;
	argc = 0;
	i = 0;
	token = (t_token *)tmp->content;
	while (tmp && (token->type == WORD || token->type == DOUBLE_QUOTE || token->type == SINGLE_QUOTE))
	{
		argc++;
		if(tmp->next == NULL)
			break ;
		tmp = tmp->next;
		token = (t_token *)tmp->content;
	}
	token = (t_token *)(*tokens)->content;
	node->argv = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		
		node->argv[i] = strdup(token->value);
		malloc_protection(node->argv[i]);
		i++;
		*tokens = (*tokens)->next;
		if (*tokens)
			token = (t_token *)(*tokens)->content;
	}
	node->argv[i] = NULL;
	return (argc);
}

void	parse_args(t_list **tokens, t_parse_node *node)
{
	t_token	*token;

	token = (t_token *)(*tokens)->content;
	if (token->type == WORD || token->type == DOUBLE_QUOTE 
		|| token->type == SINGLE_QUOTE)
		get_args(tokens, node);
	if (!*tokens)
		return ;
	parse_redirects(tokens, node);
}



void	parse_command(t_list *tokens, t_data *data)
{
	t_parse_node	*node;
	t_token			*token;

	if (tokens)
		token = (t_token *)tokens->content;
	node = create_parse_node();
	if (tokens)
	{
		parse_redirects(&tokens, node);
		if (get_builtin_index(token->value) != -1)
			node->is_builtin = true;
		else if (access(token->value, X_OK) == 0)
			node->exec = ft_strdup(token->value);
		if (tokens)
			parse_args(&tokens, node);
	}
	if (tokens == NULL || ((t_token *)tokens->content)->type != PIPE)
		node->is_last = true;
	ft_lstadd_back(&data->node_list, ft_lstnew(node));
	if (tokens)
	{
		token = (t_token *)tokens->content;
		parse_pipe(&tokens, node, data);
	}
}

void	parse_pipe(t_list **tokens, t_parse_node *node, t_data *data)
{
	t_token	*token;

	(void)node;
	token = (t_token *)(*tokens)->content;
	if (token->type == PIPE)
	{
		*tokens = (*tokens)->next;
		parse_command(*tokens, data);
	}
}

void	parse(t_data *data, char *cmd)
{
	t_list	*tokens;
	t_list	*head;
	t_token	*token;
	tokens = tokenize(cmd);
	head = tokens;
	while(tokens)
	{
		token = (t_token *)tokens->content;
		if ((token->type == DOUBLE_QUOTE || token->type == WORD) && ft_strchr(token->value, '$'))
			expand_env(token, ft_strchr(token->value, '$'), *data);	
		if(token->type == DOUBLE_QUOTE || token->type == SINGLE_QUOTE)
			remove_quotes(token);
		tokens = tokens->next;
	}
	tokens = head;
	parse_command(tokens, data);
}

char *add_until_env(char *start, char *expanded_str)
{
	size_t len;
	char *temp;
	len = count_to_next_env(start);
	if (len)
	{
		temp = ft_substr(start, 0, len);
		malloc_protection(temp);
		expanded_str = ft_strjoin2(expanded_str, temp);
		malloc_protection(expanded_str);
		free(temp);
	}
	return (expanded_str);
}

void expand_env(t_token *token, char *envpointer, t_data data)
{
	char	*expanded_str;
	char	*temp;
	char	*env_var;
	size_t	len;
	
	expanded_str = ft_strdup("");
	expanded_str = add_until_env(token->value, expanded_str);
	while (envpointer)
	{
		len = count_env_len(envpointer + 1);
		temp = ft_substr(envpointer, 1, len);
		malloc_protection(temp);
		if(temp[0] == '?')
			env_var = ft_itoa(data.last_exit_code);
		else
			env_var = envp_get(data.envp, temp);
		free(temp);
		if (env_var)
		{
			expanded_str = ft_strjoin2(expanded_str, env_var);
			malloc_protection(expanded_str);
		}
		if(token->type == DOUBLE_QUOTE)
			expanded_str = add_until_env(envpointer + len, expanded_str);
		envpointer = ft_strchr(envpointer + 1, '$');
	}
	token->value = ft_strdup(expanded_str);
	if(token->type == DOUBLE_QUOTE)
		remove_quotes(token);
}

// void	expand_envs(t_list *tokens, t_data *data)
// {
// 	t_token	*token;
// 	char	*expanded;
// 	char	*temp;
// 	char	*temp2;
// 	char	*envpointer;
// 	size_t	len;

// 	// char **subtokens;
// 	token = (t_token *)tokens->content;
// 	while (tokens)
// 	{
// 		envpointer = ft_strchr(token->value, '$');
// 		token = (t_token *)tokens->content;
// 		// if(token->type == END)
// 		// break ;
// 		if (token->type == WORD && token->value[0] != '\'' && envpointer)
// 		{
// 			len = count_to_next_env(token->value);
// 			expanded = ft_strdup("");
// 			if (len)
// 			{
// 				temp = ft_substr(token->value, 0, len);
// 				expanded = ft_strjoin(expanded, temp);
// 				free(temp);
// 			}
// 			while (envpointer)
// 			{
// 				len = count_env_len(envpointer + 1);
// 				if (len == 0)
// 				{
// 					temp = ft_strdup("$");
// 				}
// 				else if (len == 1)
// 				{
// 					
// 					else if (envpointer[1] == '$')
// 						temp = ft_itoa(getpid());
// 				}
// 				else
// 				{
// 					temp = ft_substr(envpointer, 1, len);
// 					temp2 = envp_get(data->envp, temp);
// 					if (temp2)
// 						expanded = ft_strjoin(expanded, temp2);
// 				}
// 				if (temp)
// 				{
// 					expanded = ft_strjoin(expanded, temp);
// 					free(temp);
// 				}
// 				if (token->value[0] == '\"' && envpointer)
// 				{
// 					envpointer += len + 1;
// 					len = count_to_next_env(envpointer);
// 					if (len)
// 					{
// 						temp = ft_substr(envpointer, 0, len);
// 						expanded = ft_strjoin(expanded, temp);
// 						free(temp);
// 					}
// 				}
// 				envpointer = ft_strchr(envpointer + 1, '$');
// 			}
// 			token->value = ft_strdup(expanded);
// 			free(expanded);
// 		}
// 		tokens = tokens->next;
// 	}
// }

size_t	count_env_len(char *env_var)
{
	size_t	i;

	i = 0;
	while (env_var[i] && (ft_isalnum(env_var[i]) || env_var[i] == '_'
			|| env_var[0] == '?' || env_var[1] == '$')
		&& !ft_isdigit(env_var[0]))
		i++;
	return (i);
}

size_t	count_to_next_env(char *start)
{
	size_t	i;

	i = 0;
	while (start[i] && start[i] != '$')
		i++;
	return (i);
}

void remove_quotes(t_token *token)
{
	char	*temp;
	
	temp = token->value;
	token->value = ft_substr(temp, 1, ft_strlen(temp) - 2);
	free(temp);
	malloc_protection(token->value);
}

void	print_argv(t_parse_node *node)
{
	if (node->argv == NULL)
	{
		printf("argv is NULL\n");
		return ;
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
