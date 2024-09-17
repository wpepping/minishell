/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:15:12 by phartman          #+#    #+#             */
/*   Updated: 2024/09/17 14:18:51 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delete_leading_token(t_list **tokens, t_list **current);
static void	free_token(void *content);

int	append_token(t_list **token_list, t_token_type type, char *cmd, int len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	malloc_protection(token);
	token->inword = false;
	token->value = ft_substr(cmd, 0, len);
	malloc_protection(token->value);
	token->type = type;
	ft_safelst_add_back(token, token_list);
	return (len);
}

static void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (token->value)
		free(token->value);
	free(token);
}

void	clear_tokens_list(t_list **tokens)
{
	if (tokens && *tokens)
	{
		ft_lstclear(tokens, free_token);
		*tokens = NULL;
	}
}

static void	delete_leading_token(t_list **tokens, t_list **current)
{
	t_token	*token;

	token = (t_token *)(*current)->content;
	if (token->value[0] == '\0')
	{
		if (*current == *tokens)
		{
			*tokens = (*current)->next;
			ft_lstdelone(*current, free_token);
			*current = *tokens;
			if (*tokens == NULL)
				return ;
		}
	}
}

void	combine_inword(t_list **tokens)
{
	t_token	*token;
	t_token	*next_token;
	t_list	*current;
	t_list	*next;

	current = *tokens;
	delete_leading_token(tokens, &current);
	while (*tokens != NULL && current->next != NULL)
	{
		token = (t_token *)current->content;
		next_token = (t_token *)current->next->content;
		if ((token->inword && next_token->type == WORD)
			|| next_token->value[0] == '\0')
		{
			token->inword = next_token->inword;
			token->value = ft_strjoin2(token->value, next_token->value);
			malloc_protection(token->value);
			next = current->next;
			current->next = current->next->next;
			ft_lstdelone(next, free_token);
		}
		else
			current = current->next;
	}
}
