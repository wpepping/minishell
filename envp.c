/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:31:50 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/24 13:19:16 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**envp_create(char **envp)
{
	char	**result;
	int		i;

	i = 0;
	while (envp[i++])
		;
	result = malloc((i + 2) * sizeof(char *));
	i = 0;
	while (envp[i])
	{
		result[i] = ft_strdup(envp[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}

void	envp_add(char ***envp, char *value, int n)
{
	char	**new;
	int		i;

	new = malloc((n + 2) * sizeof(char *));
	i = 0;
	while (i <= n)
	{
		new[i] = (*envp)[i];
		i++;
	}
	new[i] = value;
	new[i + 1] = NULL;
	*envp = new;
}

void	envp_remove(char ***envp, char *name)
{
	char	**new;
	int		i;
	int		n;
	char	*needle;

	needle = ft_strjoin(name, "=");
	i = -1;
	n = 0;
	while (envp[++i])
	{
		if (ft_strncmp((*envp)[i], needle, ft_strlen(needle)) != 0)
			n++;
	}
	new = malloc((n + 1) * sizeof(char *));
	i = -1;
	n = 0;
	while (envp[++i])
	{
		if (ft_strncmp((*envp)[i], needle, ft_strlen(needle)) != 0)
			new[n++] = (*envp)[i];
	}
	free(needle);
	new[i] = NULL;
	*envp = new;
}

void	envp_set(char **envp, char *value)
{
	int	cmplen;
	int	i;

	cmplen = 0;
	while (value[cmplen] != '=' && value[cmplen] != '\0')
		cmplen++;
	cmplen++;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], value, cmplen) == 0)
			break ;
		i++;
	}
	if (envp[i])
		envp[i] = value;
	else
		envp_add(&envp, value, i - 1);
}

char	*envp_get(char **envp, char *name)
{
	int		i;
	int		strlen;
	char	*needle;

	needle = ft_strjoin(name, "=");
	strlen = ft_strlen(needle);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], needle, strlen))
		{
			free(needle);
			return (ft_strchr(envp[i], '=') + 1);
		}
		i++;
	}
	free(needle);
	return (NULL);
}
