/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_envs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:53:56 by phartman          #+#    #+#             */
/*   Updated: 2024/09/17 16:19:34 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static size_t	count_to_next_env(char *start);
static char		*add_until_env(char *start, char *expanded_str);
static size_t	count_env_len(char *env_var);

void	expand_env(t_token *token, char *envpointer, t_data data)
{
	char	*expanded_str;
	char	*env_var;
	size_t	len;

	expanded_str = ft_strdup("");
	malloc_protection(expanded_str);
	expanded_str = add_until_env(token->value, expanded_str);
	while (envpointer)
	{
		len = count_env_len(envpointer + 1);
		env_var = handle_env(envpointer + 1, data, len);
		if (env_var)
		{
			expanded_str = ft_strjoin2(expanded_str, env_var);
			malloc_protection(expanded_str);
			free(env_var);
		}
		expanded_str = add_until_env(envpointer + len + 1, expanded_str);
		envpointer = ft_strchr(envpointer + 1, '$');
	}
	free(token->value);
	token->value = ft_strdup(expanded_str);
	malloc_protection(token->value);
	free(expanded_str);
}

static char	*add_until_env(char *start, char *expanded_str)
{
	size_t	len;
	char	*temp;

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

static size_t	count_env_len(char *env_var)
{
	size_t	i;

	i = 0;
	if (env_var[0] == '?')
		return (1);
	while (env_var[i] && (ft_isalnum(env_var[i]) || env_var[i] == '_'
			|| env_var[0] == '?' || env_var[0] == '$')
		&& !ft_isdigit(env_var[0]))
		i++;
	return (i);
}

static size_t	count_to_next_env(char *start)
{
	size_t	i;

	i = 0;
	while (start[i] && start[i] != '$')
		i++;
	return (i);
}
