/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:31:50 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/23 21:09:25 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	envp_add_var(char ***envp, char *value, int n)
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

void	envp_set_var(char **envp, char *value)
{
	int	cmplen;
	int	i;

	cmplen = 0;
	while (value[cmplen] != '=' && value[cmplen] != '\0')
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
		envp_add_var(&envp, value, i - 1);
}

char	*envp_get_var(char **envp, char *name)
{
	int		i;
	int		strlen;
	char	*needle;

	needle = ft_strjoin(name, "=");
	strlen = ft_strlen(needle);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, strlen))
		{
			return (ft_strchr(envp[i], '=') + 1);
		}
		i++;
	}
	return (NULL);
	free(needle);
}
