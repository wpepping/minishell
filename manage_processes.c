/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:53:43 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/21 22:06:20 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	err_handl(char *msg, char *fname, t_data *data, t_exec_node *node)
{
	if (msg != NULL)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		if (fname != NULL)
			ft_putstr_fd(node->parse->argv[0], STDERR_FILENO);
		ft_putendl_fd("", STDERR_FILENO);
	}
	cleanup(data, node, node->parse);
	exit(1);
}

static int	get_file_fd(t_data *d, t_exec_node *node, t_list *files, int oflag)
{
	char	*fname;
	int		mode;
	int		fd;

	if (oflag & O_RDONLY)
		mode = R_OK;
	else
		mode = W_OK;
	while (files)
	{
		fname = files->content;
		if (access(fname, F_OK))
			err_handl("no such file or directory: ", fname, d, node);
		else if (access(fname, mode) != 0)
			err_handl("permission denied: ", fname, d, node);
		files = files->next;
	}
	fd = open(fname, oflag);
	if (fd == -1)
		err_handl("permission denied: ", fname, d, node);
	return (fd);
}

static void	get_fds(t_data *data, t_exec_node *node, int **pipes)
{
	if (node->parse->input_src)
		node->fd_in = get_file_fd(data, node, node->parse->input_src, O_RDONLY);
	else if (node->pindex == 0)
		node->fd_in = STDIN_FILENO;
	else
		node->fd_in = pipes[node->pindex - 1][0];
	if (node->parse->output_dest)
		node->fd_out = get_file_fd(data, node,
				node->parse->output_dest, O_WRONLY);
	else if (node->parse->is_last)
		node->fd_out = STDOUT_FILENO;
	else
		node->fd_out = pipes[node->pindex][1];
}

static pid_t	forkproc(t_data *d, t_parse_node *pnode, int **pipes, int i)
{
	pid_t		pid;
	t_exec_node	enode;

	pid = fork();
	if (pid < 0)
		err_handl("failed to create process: ", pnode->argv[0], d, &enode);
	else if (pid == 0)
	{
		enode.parse = pnode;
		enode.pipes = pipes;
		enode.pindex = i;
		get_fds(d, &enode, pipes);
		dup2(enode.fd_in, STDIN_FILENO);
		dup2(enode.fd_out, STDOUT_FILENO);
		if (pnode->is_builtin)
			runbuiltin(d, &enode);
		else
			runcmd(d, &enode);
	}
	else
		return (pid);
	return (0);
}

pid_t	*fork_processes(t_data *data, t_list *lst, int lsize)
{
	int		i;
	int		**pipes;
	pid_t	*pids;

	pipes = create_pipes(lsize - 1);
	pids = malloc(lsize * sizeof(pid_t));
	i = 0;
	while (lst != NULL)
	{
		pids[i] = forkproc(data, lst->content, pipes, i);
		lst = lst->next;
		i++;
	}
	close_fds(-1, -1, pipes);
	return (pids);
}
