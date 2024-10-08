/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 19:37:20 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 16:19:34 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_pathjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (s1[len1 - 1] == '/')
		result = malloc((len1 + len2 + 1) * sizeof(char));
	else
		result = malloc((len1 + len2 + 2) * sizeof(char));
	if (result == NULL)
		return (NULL);
	ft_memcpy(result, s1, len1);
	if (s1[len1 - 1] != '/')
	{
		result[len1] = '/';
		len1++;
	}
	ft_memcpy(result + len1, s2, len2 + 1);
	return (result);
}

char	**get_path(t_data *data)
{
	char	*path_var;

	path_var = envp_get(data->envp, "PATH");
	if (path_var == NULL)
		return (ft_split("", ':'));
	return (ft_split(path_var, ':'));
}

char	*find_full_path(char *cmd, char *path[])
{
	char	*fullcmd;

	if (ft_strchr(cmd, '/') != NULL)
		return (ft_strdup(cmd));
	if (!path)
		return (NULL);
	while (*path)
	{
		fullcmd = ft_pathjoin(*path, cmd);
		if (access(fullcmd, X_OK) == 0)
			return (fullcmd);
		free(fullcmd);
		path++;
	}
	return (NULL);
}
