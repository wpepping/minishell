/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:53:43 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/20 19:37:16 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	err_handl(char *msg, t_data *data, t_exec_node *node)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putendl_fd(node->parse->argv[0], STDERR_FILENO);
	cleanup(data, node);
	return (1);
}

static void	get_fds(t_data *data, t_exec_node *node)
{

}

static void	runproc(t_data *data, t_exec_node *node)
{
	dup2(node->fd_in, STDIN_FILENO);
	dup2(node->fd_out, STDOUT_FILENO);
	if (node->parse->is_builtin)
		runbuiltin(data, node);
	else
		runcmd(data, node);
}

static pid_t	forkproc(t_data *data, t_parse_node *pnode)
{
	pid_t		pid;
	t_exec_node	enode;

	pid = fork();
	enode.parse = pnode;
	get_fds(data, &enode);
	if (pid < 0)
		err_handl("failed to create process: ", data, &enode);
	if (pid == 0)
		runproc(data, &enode);
	else
		return (pid);
	return (0);
}

pid_t	*fork_processes(t_data *data, t_list *lst)
{
	int		i;
	pid_t	*pids;

	i = 0;
	while (lst != NULL)
	{
		pids[i] = forkproc(data, lst);
		lst = lst->next;
	}
}
