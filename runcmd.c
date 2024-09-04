/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 21:20:00 by wouter            #+#    #+#             */
/*   Updated: 2024/09/04 21:16:27 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	runbuiltin(t_data *data, t_exec_node *node)
{
	close_fds(node->fd_in, node->fd_out, node->pipes);
	if (ft_strncmp(node->parse->argv[0], "echo", 5) == 0)
		return (ft_echo(data, node));
	if (ft_strncmp(node->parse->argv[0], "cd", 3) == 0)
		return (ft_cd(data, node));
	if (ft_strncmp(node->parse->argv[0], "pwd", 4) == 0)
		return (ft_pwd(data, node));
	if (ft_strncmp(node->parse->argv[0], "export", 7) == 0)
		return (ft_export(data, node));
	if (ft_strncmp(node->parse->argv[0], "unset", 6) == 0)
		return (ft_unset(data, node));
	if (ft_strncmp(node->parse->argv[0], "env", 4) == 0)
		return (ft_env(data, node));
	if (ft_strncmp(node->parse->argv[0], "exit", 5) == 0)
		return (ft_exit(data, node));
	return (0);
}

void	runcmd(t_data *data, t_exec_node *node)
{
	char	**argv;

	argv = node->parse->argv;
	node->parse->argv = NULL;
	close_fds(node->fd_in, node->fd_out, node->pipes);
	free_array((void **)node->pipes);
	cleanup_cmd(node->parse_nodes);
	execve(node->fullcmd, argv, data->envp);
	ft_putstrs_fd("minishell: ", argv[0], ERR_CANNOT_EXEC, STDERR_FILENO);
	exit(1); // Change to clean exit?
}
