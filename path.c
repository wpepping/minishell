/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 19:37:20 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/04 19:39:56 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path(void)
{
	char	*path_var;

	path_var = getenv("PATH");
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
		path++;
	}
	return (NULL);
}
