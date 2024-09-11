/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 19:28:53 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/11 17:20:28 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	err_handl(char *msg, char *fname, t_data *data, t_exec_node *node)
{
	if (msg != NULL)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		if (fname != NULL)
			ft_putstr_fd(fname, STDERR_FILENO);
		ft_putendl_fd("", STDERR_FILENO);
	}
	if (!node->nofork)
		clean_exit(data, node, node->parse_nodes, 1);
	return (-1);
}

static int	get_file_fd(t_data *d, t_exec_node *node, t_token *file)
{
	char	*fname;
	int		fd;
	int		oflag;

	oflag = oflags(file->type);
	fname = file->value;
	fd = open(fname, oflag, 0644);
	if (fd == -1 && !node->nofork)
		clean_exit(d, node, node->parse_nodes, node->error_code);
	return (fd);
}

void	get_fds(t_data *data, t_exec_node *node, int **pipes)
{
	if (node->infile)
		node->fd_in = get_file_fd(data, node, node->infile);
	else if (node->pindex == 0)
		node->fd_in = STDIN_FILENO;
	else
		node->fd_in = pipes[node->pindex - 1][0];
	if (node->outfile)
		node->fd_out = get_file_fd(data, node, node->outfile);
	else if (node->parse->is_last)
		node->fd_out = STDOUT_FILENO;
	else
		node->fd_out = pipes[node->pindex][1];
}

static pid_t	forkproc(t_data *d, t_exec_node *enode, t_parse_node *pnode)
{
	pid_t		pid;

	pid = fork();
	if (pid < 0)
		err_handl("failed to create process: ", pnode->argv[0], d, enode);
	else if (pid == 0)
	{
		if (!enode->run_cmd)
			clean_exit(d, enode, enode->parse_nodes, enode->error_code);
		sigaction(SIGINT, NULL, NULL);
		get_fds(d, enode, enode->pipes);
		dup2(enode->fd_in, STDIN_FILENO);
		dup2(enode->fd_out, STDOUT_FILENO);
		if (pnode->is_builtin)
			clean_exit(d, enode, enode->parse_nodes, runbuiltin(d, enode));
		runcmd(d, enode);
	}
	else
		return (pid);
	return (0);
}

pid_t	*fork_processes(t_data *data, t_execution *exec)
{
	int			i;
	pid_t		*pids;
	t_list		*enodes;

	pids = malloc(exec->lsize * sizeof(pid_t));
	enodes = exec->enodes;
	i = 0;
	while (enodes != NULL)
	{
		pids[i] = forkproc(data, enodes->content,
				((t_exec_node *)enodes->content)->parse);
		enodes = enodes->next;
		i++;
	}
	close_fds(-1, -1, exec->pipes);
	ft_lstclear(&enodes, free);
	return (pids);
}
