/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 20:03:10 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/18 20:17:51 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(t_data *data, char *cmd)
{
	char	*arg;
	char	*base;
	char	*path;

	arg = ft_substr(cmd, 3, ft_strlen(cmd));
	if (arg[0] == '/')
		base = ft_strdup("");
	else
		base = ft_strjoin(data->cwd, "/");
	path = ft_strjoin(base, arg);
	chdir(path);
	free(base);
	free(path);
	getcwd(data->cwd, PATH_MAX);
}
