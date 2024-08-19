/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 20:03:10 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/19 15:48:28 by phartman         ###   ########.fr       */
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

void echo(char *cmd)
{
	int newline;
	char *arg;
	
	newline = 1;
	if (ft_strncmp(cmd, "echo -n", 7) == 0)
	{
		newline = 0;
		arg = ft_substr(cmd, 7, ft_strlen(cmd));
	}
	arg = ft_substr(cmd, 5, ft_strlen(cmd));
	printf("%s", arg);
	if (newline)
		printf("\n");
}
