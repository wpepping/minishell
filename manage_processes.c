/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 19:28:53 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/04 21:44:55 by wpepping         ###   ########.fr       */
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
		clean_exit(data, node, node->parse_nodes);
	return (-1);
}

static int	get_file_fd(t_data *d, t_exec_node *node, t_list *files, int oflag)
{
	char	*fname;
	int		fd;
	int		type;

	while (files->next)
		files = files->next;
	type = ((t_token *)files->content)->type;
	fname = ((t_token *)files->content)->value;
	if (type == APPEND)
		oflag = oflag | O_APPEND;
	fd = open(fname, oflag, 0644);
	if (fd == -1 && !node->nofork)
		clean_exit(d, node, node->parse_nodes);
	return (fd);
}

void	get_fds(t_data *data, t_exec_node *node, int **pipes)
{
	int	oflag;

	if (node->parse->input_src)
		node->fd_in = get_file_fd(data, node, node->parse->input_src, O_RDONLY);
	else if (node->pindex == 0)
		node->fd_in = STDIN_FILENO;
	else
		node->fd_in = pipes[node->pindex - 1][0];
	if (node->parse->output_dest)
	{
		if (((t_token *)node->parse->output_dest)->type == APPEND)
			oflag = O_CREAT | O_WRONLY | APPEND;
		else
			oflag = O_CREAT | O_WRONLY | O_TRUNC;
		node->fd_out = get_file_fd(data, node, node->parse->output_dest, oflag);
	}
	else if (node->parse->is_last)
		node->fd_out = STDOUT_FILENO;
	else
		node->fd_out = pipes[node->pindex][1];
}

static pid_t	forkproc(t_data *d, t_exec_node *enode, t_parse_node *pnode)
{
	pid_t		pid;
	int			return_value;

	pid = fork();
	if (pid < 0)
		err_handl("failed to create process: ", pnode->argv[0], d, enode);
	else if (pid == 0)
	{
		sigaction(SIGINT, NULL, NULL);
		get_fds(d, enode, enode->pipes);
		dup2(enode->fd_in, STDIN_FILENO);
		dup2(enode->fd_out, STDOUT_FILENO);
		if (!enode->run_cmd)
			exit(enode->error_code);
		else if (pnode->is_builtin)
		{
			return_value = runbuiltin(d, enode);
			exit(return_value); // Change to clean exit?
		}
		else
			runcmd(d, enode);
	}
	else
		return (pid);
	return (0);
}

pid_t	*fork_processes(t_data *data, t_list *pnodes, int lsize)
{
	int			i;
	pid_t		*pids;
	t_list		*enodes;
	t_execution	execution;

	execution.pnodes = pnodes;
	execution.lsize = lsize;
	execution.nofork = 0;
	execution.enodes = NULL;
	execution.pipes = create_pipes(execution.lsize - 1); // Deal with NULL
	enodes = create_exec_nodes(data, &execution);
	pids = malloc(lsize * sizeof(pid_t));
	i = 0;
	while (enodes != NULL)
	{
		pids[i] = forkproc(data, enodes->content,
				((t_exec_node *)enodes->content)->parse);
		enodes = enodes->next;
		i++;
	}
	close_fds(-1, -1, execution.pipes);
	free_array((void **)execution.pipes);
	ft_lstclear(&enodes, free);
	return (pids);
}
