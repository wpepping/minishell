/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 21:20:00 by wouter            #+#    #+#             */
/*   Updated: 2024/08/24 18:43:45 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_path(void)
{
	char	*path_var;

	path_var = getenv("PATH");
	if (path_var == NULL)
		return (ft_split("", ':'));
	return (ft_split(path_var, ':'));
}

static char	*find_full_path(char *cmd, char *path[])
{
	char	*fullcmd;

	if (ft_strchr(cmd, '/') != NULL)
		return (cmd);
	while (*path)
	{
		fullcmd = ft_pathjoin(*path, cmd);
		if (access(fullcmd, X_OK) == 0)
			return (fullcmd);
		path++;
	}
	return (NULL);
}

static void	err_handl(char *message, char *cmd, t_data *data, t_exec_node *node)
{
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putendl_fd(cmd, STDERR_FILENO);
	cleanup(data, node, node->parse);
	exit(1);
}

void	runbuiltin(t_data *data, t_exec_node *node)
{
	if (ft_strncmp(node->parse->argv[0], "echo", 5) == 0)
		ft_echo(data, node);
	if (ft_strncmp(node->parse->argv[0], "cd", 5) == 0)
		ft_cd(data, node);
	if (ft_strncmp(node->parse->argv[0], "pwd", 5) == 0)
		ft_pwd(data, node);
	if (ft_strncmp(node->parse->argv[0], "export", 5) == 0)
		ft_export(data, node);
	if (ft_strncmp(node->parse->argv[0], "unset", 5) == 0)
		ft_unset(data, node);
	if (ft_strncmp(node->parse->argv[0], "env", 5) == 0)
		ft_env(data, node);
}

void	runcmd(t_data *data, t_exec_node *node)
{
	char	**path;
	char	*fullcmd;

	path = get_path();
	if (path == NULL)
		err_handl(ERR_OUT_OF_MEMORY, NULL, data, node);
	fullcmd = find_full_path(node->parse->argv[0], path);
	if (fullcmd == NULL)
	{
		free_array((void **)path);
		err_handl(ERR_COMMAND_NOT_FOUND, node->parse->argv[0], data, node);
	}
	close_fds(node->fd_in, node->fd_out, node->pipes);
	if (execve(fullcmd, node->parse->argv, data->envp) < 0)
		exit(1);
}
