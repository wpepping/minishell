/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:53:43 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/03 17:37:49 by phartman         ###   ########.fr       */
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
		clean_exit(NULL, data, node, node->parse_nodes);
	return (-1);
}

static int	get_file_fd(t_data *d, t_exec_node *node, t_list *files, int oflag)
{
	char	*fname;
	int		fd;
	int		type;

	while (files)
	{
		type = ((t_token *)files->content)->type;
		fname = ((t_token *)files->content)->value;
		fd = open(fname, oflag, 0644);
		if (fd == -1)
		{
			if ((type == REDIRECT_IN || type == HEREDOC)
				&& access(fname, F_OK) != 0)
				return (err_handl(ERR_NO_SUCH_FILE, fname, d, node));
			else
				return (err_handl(ERR_PERMISSION_DENIED, fname, d, node));
		}
		if (files->next)
			close(fd);
		files = files->next;
	}
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
	// pid_t		pid;
	int			return_value;

	// pid = fork();
	// if (pid < 0)
	// 	err_handl("failed to create process: ", pnode->argv[0], d, enode);
	// else if (pid == 0)
	// {
		get_fds(d, enode, enode->pipes);
		dup2(enode->fd_in, STDIN_FILENO);
		dup2(enode->fd_out, STDOUT_FILENO);
		if (pnode->is_builtin)
		{
			return_value = runbuiltin(d, enode);
			exit(return_value);
		}
		else
			runcmd(d, enode);
	// }
	// else
	// 	return (pid);
	return (0);
}

pid_t	*fork_processes(t_data *data, t_list *lst, int lsize)
{
	int			i;
	pid_t		*pids;
	t_exec_node	enode;

	enode.nofork = 0;
	enode.list_size = lsize;
	enode.parse_nodes = lst;
	enode.pipes = create_pipes(lsize - 1); // HANDLE NULL
	pids = malloc(lsize * sizeof(pid_t));
	i = 0;
	while (lst != NULL)
	{
		enode.pindex = i;
		enode.parse = lst->content;
		pids[i] = forkproc(data, &enode, enode.parse);
		lst = lst->next;
		i++;
	}
	close_fds(-1, -1, enode.pipes);
	free_array((void **)enode.pipes);
	return (pids);
}
