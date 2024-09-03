/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/03 15:13:47 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_redirects(t_list **tokens, t_parse_node *node)
{
	t_token	*token;

	while (*tokens != NULL)
	{
		token = (t_token *)(*tokens)->content;
		if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN
			|| token->type == APPEND || token->type == HEREDOC)
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
			break ;
		}
	}
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
	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (token->type == DOUBLE_QUOTE || token->type == SINGLE_QUOTE)
			token->value = ft_substr(token->value, 1, ft_strlen(token->value)
					- 2);
		if ((token->type == DOUBLE_QUOTE || token->type == WORD)
			&& ft_strchr(token->value, '$'))
			expand_env(token, ft_strchr(token->value, '$'), *data);
		tokens = tokens->next;
	}
	tokens = head;
	if(((t_token *)ft_lstlast(tokens)->content)->type == PIPE || ((t_token *)tokens->content)->type == PIPE)
	{
		printf("Error: syntax error near unexpected token '|'\n");
		exit(1);
	}
	parse_command(tokens, data);
	clear_tokens_list(&head);
}
