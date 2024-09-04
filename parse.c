/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/04 20:00:00 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_redirects(t_list **tokens, t_parse_node *node)
{
	t_token	*token;

	while (*tokens != NULL)
	{
		token = (t_token *)(*tokens)->content;
		if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN
			|| token->type == APPEND || token->type == HEREDOC)
		{
			if (!(*tokens)->next
				|| !is_valid_filename((*tokens)->next->content))
			{
				printf("Error: no filename specified for redirection\n");
				return (1);
			}
			*tokens = handle_redirects(*tokens, node);
		}
		else
		{
			break ;
		}
	}
	return (0);
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
}

int	parse_command(t_list *tokens, t_data *data)
{
	t_parse_node	*node;
	t_token			*token;
	int				return_value;

	return_value = 0;
	if (tokens)
		token = (t_token *)tokens->content;
	node = create_parse_node();
	if (tokens)
	{
		if (parse_redirects(&tokens, node))
			return (1);
		if (get_builtin_index(token->value) != -1)
			node->is_builtin = true;
		else
			node->exec = ft_strdup(token->value);
		if (tokens)
		{
			if (parse_redirects(&tokens, node))
				return (1);
			parse_args(&tokens, node);
			if (parse_redirects(&tokens, node))
				return (1);
		}
	}
	if (tokens == NULL || ((t_token *)tokens->content)->type != PIPE)
		node->is_last = true;
	ft_lstadd_back(&data->node_list, ft_lstnew(node));
	if (tokens)
	{
		token = (t_token *)tokens->content;
		parse_pipe(&tokens, data);
	}
	return (0);
}

int	parse_pipe(t_list **tokens, t_data *data)
{
	t_token	*token;

	token = (t_token *)(*tokens)->content;
	if (token->type == PIPE)
	{
		*tokens = (*tokens)->next;
		return (parse_command(*tokens, data));
	}
	return (0);
}

void	combine_inword(t_list **tokens)
{
	t_token	*token;
	t_token	*next_token;
	t_list	*current;
	t_list	*next;

	current = *tokens;
	while (tokens != NULL && current->next != NULL)
	{
		token = (t_token *)current->content;
		next = current->next;
		next_token = (t_token *)next->content;
		if (token->inword && next_token->inword)
		{
			token->value = ft_strjoin2(token->value, next_token->value);
			current->next = next->next;
			ft_lstdelone(next, free_token);
		}
		else
			current = current->next;
	}
}

int	parse(t_data *data, char *cmd)
{
	t_list	*tokens;
	t_list	*head;
	t_token	*token;
	int		return_value;

	return_value = tokenize(cmd, &tokens);
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
	combine_inword(&tokens);
	return_value = parse_command(tokens, data);
	if ((return_value == 0)
		&& (((t_token *)ft_lstlast(tokens)->content)->type == PIPE
			|| ((t_token *)(tokens)->content)->type == PIPE))
	{
		printf("Error: syntax error near unexpected token '|'\n");
		return_value = 1;
	}
	clear_tokens_list(&head);
	return (return_value);
}
