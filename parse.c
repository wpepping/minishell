/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/04 20:02:49 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	parse_args_and_redirects(t_list **tokens, t_parse_node *node, t_data data)
{
	t_token *token;
	t_list	*head;
	int argc;

	head = *tokens;
	argc = 0;
	while (*tokens != NULL && ((t_token *)(*tokens)->content)->type != PIPE)
	{
		token = (t_token *)(*tokens)->content;
		if (token->type == WORD)
		{
			if (ft_strncmp(token->value, "", 1) != 0)
				argc++;
			*tokens = (*tokens)->next;
		}
		else if (token->type == REDIRECT_OUT || token->type == REDIRECT_IN
			|| token->type == APPEND || token->type == HEREDOC)
		{
			if (!(*tokens)->next || !is_valid_filename((*tokens)->next->content))
			{
				printf("Error: no filename specified for redirection\n");
				return (1);
			}
			*tokens = handle_redirects(*tokens, node, data);
		}
	}
	handle_args(head, node, argc);
	return (0);
}


int	parse_command(t_list *tokens, t_data *data)
{
	t_parse_node	*node;

	
	if (tokens)
	{
		node = create_parse_node();
		parse_args_and_redirects(&tokens, node, *data);
		if (node->argv[0] != NULL)
		{
			if (get_builtin_index(node->argv[0]) != -1)
			node->is_builtin = true;
		else
			node->exec = ft_strdup(node->argv[0]);
		}
	
	if (tokens == NULL || ((t_token *)tokens->content)->type != PIPE)
		node->is_last = true;
	ft_lstadd_back(&data->node_list, ft_lstnew(node));
	if (tokens)
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
		if (token->type == DOUBLE_QUOTE || token->type == SINGLE_QUOTE)
			token->type = WORD;
		tokens = tokens->next;
	}
	tokens = head;
	combine_inword(&tokens);
	//head = tokens;
	if(return_value == 0)
		return_value = parse_command(tokens, data);
	if ((return_value == 0 && tokens != NULL)
		&& (((t_token *)ft_lstlast(tokens)->content)->type == PIPE
			|| ((t_token *)(tokens)->content)->type == PIPE))
	{
		printf("Error: syntax error near unexpected token '|'\n");
		return_value = 1;
	}
	if(tokens)
		clear_tokens_list(&tokens);
	data->last_exit_code = return_value;
	return (return_value);
}
