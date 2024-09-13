/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:07:24 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/12 18:54:55 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_tokens(t_list *tokens, t_data *data);
static int	parse_command(t_list *tokens, t_data *data);
static int	parse_pipe(t_list **tokens, t_data *data);
static int	parse_args_and_redirects(t_list **tokens, t_parse_node *node,
				t_data data);

int	parse(t_data *data, char *cmd)
{
	t_list	*tokens;
	int		return_value;

	return_value = tokenize(cmd, &tokens);
	if (!return_value)
	{
		if ((return_value == 0 && tokens != NULL)
			&& (((t_token *)ft_lstlast(tokens)->content)->type == PIPE
				|| ((t_token *)(tokens)->content)->type == PIPE))
		{
			ft_puterr(NULL, "syntax error near unexpected token '|'", NULL);
			return_value = 2;
		}
		else
		{
			process_tokens(tokens, data);
			combine_inword(&tokens);
			return_value = parse_command(tokens, data);
		}
	}
	if (tokens)
		clear_tokens_list(&tokens);
	data->last_exit_code = return_value;
	return (return_value);
}

static int	parse_args_and_redirects(t_list **tokens, t_parse_node *node,
		t_data data)
{
	t_token	*token;
	t_list	*head;
	int		argc;

	head = *tokens;
	argc = 0;
	while (*tokens != NULL && ((t_token *)(*tokens)->content)->type != PIPE)
	{
		token = (t_token *)(*tokens)->content;
		if (token->type == WORD)
		{
			argc++;
			*tokens = (*tokens)->next;
		}
		else if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN
			|| token->type == APPEND || token->type == HEREDOC)
		{
			if (!is_valid_filename((*tokens)->next))
				return (1);
			*tokens = handle_redirects(*tokens, node, data);
			if (node->heredoc_fail == true)
				return (130);
		}
	}
	handle_args(head, node, argc);
	return (0);
}

static int	parse_pipe(t_list **tokens, t_data *data)
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

static int	parse_command(t_list *tokens, t_data *data)
{
	t_parse_node	*node;
	int				return_value;

	if (tokens)
	{
		node = create_parse_node();
		return_value = parse_args_and_redirects(&tokens, node, *data);
		if (return_value)
			return (return_value);
		if (node->argv[0] != NULL && get_builtin_index(node->argv[0]) != -1)
			node->is_builtin = true;
		if (tokens == NULL || ((t_token *)tokens->content)->type != PIPE)
			node->is_last = true;
		ft_safelst_add_back(node ,&data->node_list);
		if (tokens)
			parse_pipe(&tokens, data);
	}
	return (0);
}

static void	process_tokens(t_list *tokens, t_data *data)
{
	t_token	*token;

	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (token->type == DOUBLE_QUOTE || token->type == SINGLE_QUOTE)
			token->value = ft_substr(token->value, 1, ft_strlen(token->value)
					- 2);
		if ((token->type == DOUBLE_QUOTE || token->type == WORD)
			&& ft_strchr(token->value, '$'))
			expand_env(token, ft_strchr(token->value, '$'), *data);
		if (token->type == DOUBLE_QUOTE || token->type == SINGLE_QUOTE)
			token->type = WORD;
		tokens = tokens->next;
	}
}
