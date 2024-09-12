/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:12:12 by phartman          #+#    #+#             */
/*   Updated: 2024/09/12 16:55:18 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*handle_redirects(t_list *tokens, t_parse_node *node, t_data data)
{
	t_token	*token;
	t_list	*current;
	t_token	*content_copy;

	current = tokens;
	token = (t_token *)current->content;
	content_copy = malloc(sizeof(t_token));
	malloc_protection(content_copy);
	content_copy->type = token->type;
	content_copy->value = ft_strdup(((t_token *)current->next->content)->value);
	malloc_protection(content_copy->value);
	if (token->type == REDIRECT_IN || token->type == HEREDOC
		|| token->type == REDIRECT_OUT || token->type == APPEND)
	{
		if (token->type == HEREDOC)
			if (handle_heredoc(content_copy->value, data, content_copy))
				node->heredoc_fail = true;
		ft_safelst_add_back(content_copy, &node->redirect);
	}
	if (current->next != NULL)
		return (current->next->next);
	else
		return (current->next);
}

void	handle_args(t_list *tokens, t_parse_node *node, int argc)
{
	int		i;
	t_token	*token;

	token = (t_token *)tokens->content;
	i = 0;
	node->argv = malloc(sizeof(char *) * (argc + 1));
	malloc_protection(node->argv);
	while (i < argc && tokens)
	{
		token = (t_token *)tokens->content;
		if (token->type == REDIRECT_IN || token->type == REDIRECT_OUT
			|| token->type == APPEND || token->type == HEREDOC)
		{
			tokens = tokens->next->next;
			continue ;
		}
		if (token->type == WORD)
		{
			node->argv[i] = strdup(token->value);
			malloc_protection(node->argv[i]);
			i++;
		}
		tokens = tokens->next;
	}
	node->argv[i] = NULL;
}

char	*handle_env(char *envpointer, t_data data, size_t len)
{
	char	*temp;
	char	*env_var;
	char	*env_val;

	temp = ft_substr(envpointer, 0, len);
	malloc_protection(temp);
	if (temp[0] == '?')
		env_var = ft_itoa(data.last_exit_code);
	else if (len == 0)
		env_var = ft_strdup("$");
	else
	{
		env_val = envp_get(data.envp, temp);
		if (!env_val)
		{
			free(temp);
			return (NULL);
		}
		else
			env_var = ft_strdup(env_val);
	}
	free(temp);
	malloc_protection(env_var);
	return (env_var);
}
