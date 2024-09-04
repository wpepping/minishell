/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:58:35 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/03 14:58:43 by wpepping         ###   ########.fr       */
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
	if (!result)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		result[i] = ft_strdup(envp[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	envp_add(char ***envp, char *value, int n)
{
	char	**new;
	int		i;

	new = malloc((n + 2) * sizeof(char *));
	if (!new)
		return (-1);
	i = 0;
	while (i < n)
	{
		new[i] = (*envp)[i];
		i++;
	}
	new[i] = value;
	new[i + 1] = NULL;
	*envp = new;
	return (0);
}

char	**envp_remove(char **envp, char **names)
{
	char	**new;
	int		i;
	int		n;

	i = -1;
	n = 0;
	while (envp[++i])
	{
		if (!arrncontains(names, envp[i], ft_strchr(envp[i], '=') - envp[i]))
			n++;
	}
	new = malloc((n + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	i = -1;
	n = 0;
	while (envp[++i])
	{
		if (!arrncontains(names, envp[i], ft_strchr(envp[i], '=') - envp[i]))
			new[n++] = envp[i];
	}
	new[n] = NULL;
	return (new);
}

int	envp_set(char ***envp, char *value)
{
	int		cmplen;
	int		i;
	char	**temp;

	cmplen = 0;
	while (value[cmplen] != '=' && value[cmplen] != '\0')
		cmplen++;
	cmplen++;
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], value, cmplen) == 0)
			break ;
		i++;
	}
	if ((*envp)[i])
		(*envp)[i] = value;
	else
	{
		temp = *envp;
		if (envp_add(envp, value, i))
			return (-1);
		free(temp);
	}
	return (0);
}

char	*envp_get(char **envp, char *name)
{
	int		i;
	int		strlen;
	char	*needle;

	needle = ft_strjoin(name, "=");
	if (!needle)
		return (NULL);
	strlen = ft_strlen(needle);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], needle, strlen) == 0)
		{
			free(needle);
			return (ft_strchr(envp[i], '=') + 1);
		}
		i++;
	}
	free(needle);
	return (NULL);
}
