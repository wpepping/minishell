/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 19:37:17 by phartman         ###   ########.fr       */
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

void	handle_redirects(t_list **tokens, t_parse_node *node)
{
	t_token	*token;
	t_list	*current;

	token = (t_token *)(*tokens)->content;
	current = *tokens;
	while (token->type != PIPE && (t_token *)current->next != NULL)
	{
		token = (t_token *)current->content;
		if ((token->type == APPEND))
		{
			ft_lstadd_back(&node->output_dest,
				ft_lstnew(ft_strdup(((t_token *)current->next->content)->value)));
			node->append = true;
		}
		else if (token->type == REDIRECT_OUT)
			ft_lstadd_back(&node->output_dest,
				ft_lstnew(ft_strdup(((t_token *)current->next->content)->value)));
		else if (token->type == REDIRECT_IN)
			ft_lstadd_back(&node->input_src,
				ft_lstnew(ft_strdup(((t_token *)current->next->content)->value)));
		// else if (token->type == HEREDOC)
		// node->heredoc = true;
		if (token->type == PIPE)
		{
			*tokens = current;
			return ;
		}
		if (current->next != NULL && current->next->next != NULL)
			current = current->next->next;
		else
			current = current->next;
	}
	*tokens = current;
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
	while (tmp && ((t_token *)tmp->content)->type == WORD)
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
		if (*tokens)
			token = (t_token *)(*tokens)->content;
	}
	node->argv[i] = NULL;
	return (argc);
}

void	parse_args_and_redirects(t_list **tokens, t_parse_node *node)
{
	t_token	*token;

	token = (t_token *)(*tokens)->content;
	if (token->type == WORD)
		get_args(tokens, node);
	if (!*tokens)
		return ;
	token = (t_token *)(*tokens)->content;
	if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN
		|| token->type == APPEND)
		handle_redirects(tokens, node);
}

void	parse_command(t_list *tokens, t_data *data)
{
	t_parse_node	*node;
	t_token			*token;

	if (tokens)
		token = (t_token *)tokens->content;
	node = create_parse_node();
	if (token->type == WORD && tokens)
	{	
		if (get_builtin_index(token->value) != -1)
			node->is_builtin = true;
		else if (access(token->value, X_OK) == 0)
			node->exec = ft_strdup(token->value);
		if (tokens)
			parse_args_and_redirects(&tokens, node);
	}
	//if (!tokens)
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
	
	

	tokens = tokenize(cmd);
	expand_envs(tokens);
	parse_command(tokens, data);
}

void expand_envs(t_list *tokens)
{
	t_token *token;
	char *env_var;
	char *expanded;
	int i;

	token = (t_token *)tokens->content;
	
	while(token->type != END)
	{
		if(ft_strchr(token->value, '\'') != NULL)
		{
			tokens = tokens->next;
			continue;
		}	
		if (token->type == WORD)
		{
			i = 0;
			env_var = ft_strchr(token->value, '$');
			while(env_var[i] && env_var[i] != ' ' && env_var[i] != '\t')
				i++;
			env_var = ft_substr(env_var, 1, i);
			expanded = ft_strdup(getenv(env_var));
			free(token->value);
			token->value = expanded;
			free(env_var);
		}
		tokens = tokens->next;
		if(tokens)
			token = (t_token *)tokens->content;
	}
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
