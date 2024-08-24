/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 20:03:10 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/24 18:55:00 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_data *data, t_exec_node *node)
{
	(void)data;
	(void)node;
}

void	ft_cd(t_data *data, t_exec_node *node)
{
	char	*arg;
	char	*base;
	char	*path;

	arg = node->parse->argv[1];
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

void	ft_pwd(t_data *data, t_exec_node *node)
{
	(void)node;
	ft_putendl_fd(data->cwd, 1);
}

void	ft_export(t_data *data, t_exec_node *node)
{
	envp_set(&data->envp, node->parse->argv[1]);
}

void	ft_unset(t_data *data, t_exec_node *node)
{
	char	**temp;

	temp = envp_remove(data->envp, node->parse->argv[1]);
	free(data->envp);
	data->envp = temp;
	(void)data;
	(void)node;
}
//amogus plz don't delete me :3
