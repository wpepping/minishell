/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 21:20:00 by wouter            #+#    #+#             */
/*   Updated: 2024/09/04 14:19:52 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cmd_err_handl(char *message, char *cmd,
	t_data *data, t_exec_node *node)
{
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putendl_fd(cmd, STDERR_FILENO);
	clean_exit(data, node, node->parse_nodes);
}

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
		return (ft_strdup(cmd));
	while (*path)
	{
		fullcmd = ft_pathjoin(*path, cmd);
		if (access(fullcmd, X_OK) == 0)
			return (fullcmd);
		path++;
	}
	return (NULL);
}

int	runbuiltin(t_data *data, t_exec_node *node)
{
	close_fds(node->fd_in, node->fd_out, node->pipes);
	if (ft_strncmp(node->parse->argv[0], "echo", 5) == 0)
		return (ft_echo(data, node));
	if (ft_strncmp(node->parse->argv[0], "cd", 2) == 0)
		return (ft_cd(data, node));
	if (ft_strncmp(node->parse->argv[0], "pwd", 3) == 0)
		return (ft_pwd(data, node));
	if (ft_strncmp(node->parse->argv[0], "export", 6) == 0)
		return (ft_export(data, node));
	if (ft_strncmp(node->parse->argv[0], "unset", 5) == 0)
		return (ft_unset(data, node));
	if (ft_strncmp(node->parse->argv[0], "env", 3) == 0)
		return (ft_env(data, node));
	if (ft_strncmp(node->parse->argv[0], "exit", 4) == 0)
		return (ft_exit(data, node));
	return (0);
}

void	runcmd(t_data *data, t_exec_node *node)
{
	char	**path;
	char	*fullcmd;
	char	**argv;

	argv = node->parse->argv;
	node->parse->argv = NULL;
	path = get_path();
	if (path == NULL)
		cmd_err_handl(ERR_OUT_OF_MEMORY, NULL, data, node);
	fullcmd = find_full_path(argv[0], path);
	free_array((void **)path);
	if (fullcmd == NULL)
		cmd_err_handl(ERR_COMMAND_NOT_FOUND, argv[0], data, node);
	if (isdir(fullcmd))
		cmd_err_handl(ERR_IS_DIR, argv[0], data, node);
	if (access(fullcmd, X_OK) != 0)
		cmd_err_handl(ERR_PERMISSION_DENIED, argv[0], data, node);
	close_fds(node->fd_in, node->fd_out, node->pipes);
	free_array((void **)node->pipes);
	cleanup_cmd(node->parse_nodes);
	execve(fullcmd, argv, data->envp);
	ft_putstrs_fd("minishell: ", argv[0], ERR_CANNOT_EXEC, STDERR_FILENO);
	exit(1); // Change to clean exit?
}
