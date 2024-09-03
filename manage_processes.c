/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_processes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:53:43 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/03 16:59:29 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	err_handl(char *msg, char *fname, t_data *data, t_exec_node *node)
{
	if (msg != NULL)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		if (fname != NULL)
			ft_putstr_fd(fname, STDERR_FILENO);
		ft_putendl_fd("", STDERR_FILENO);
	}
	if (!node->nofork)
		clean_exit(NULL, data, node, node->parse);
	cleanup_cmd(data, node, node->parse);
}

static int	get_file_fd(t_data *d, t_exec_node *node, t_list *files, int oflag)
{
	char	*fname;
	int		fd;

	while (files)
	{
		//printf("get file fd for: %s\n", (char *)(files->content));
		fname = ((t_token *)files->content)->value;
		fd = open(fname, oflag, 0644);
		if (fd == -1)
		{
			if (oflag == O_RDONLY && access(fname, F_OK) != 0)
				err_handl("minishell: no such file or directory: ", fname, d, node);
			else
				err_handl("minishell: permission denied: ", fname, d, node);
		}
		if (files->next)
			close(fd);
		files = files->next;
	}
	return (fd);
}

void	get_fds(t_data *data, t_exec_node *node, int **pipes)
{
	if (node->parse->input_src)
		node->fd_in = get_file_fd(data, node, node->parse->input_src, O_RDONLY);
	else if (node->pindex == 0)
		node->fd_in = STDIN_FILENO;
	else
		node->fd_in = pipes[node->pindex - 1][0];
	if (node->parse->output_dest)
		node->fd_out = get_file_fd(data, node,
				node->parse->output_dest, O_CREAT | O_WRONLY | O_TRUNC);
	else if (node->parse->is_last)
		node->fd_out = STDOUT_FILENO;
	else
		node->fd_out = pipes[node->pindex][1];
	//printf("process %i fd_in %i fd_out %i\n", node->pindex, node->fd_in, node->fd_out);
}

static pid_t	forkproc(t_data *d, t_parse_node *pnode, int **pipes, int i)
{
	pid_t		pid;
	t_exec_node	enode;
	int			return_value;

	pid = fork();
	if (pid < 0)
		err_handl("failed to create process: ", pnode->argv[0], d, &enode);
	else if (pid == 0)
	{
		enode.parse = pnode;
		enode.pipes = pipes;
		enode.pindex = i;
		enode.nofork = 0;
		get_fds(d, &enode, pipes);
		dup2(enode.fd_in, STDIN_FILENO);
		dup2(enode.fd_out, STDOUT_FILENO);
		if (pnode->is_builtin)
		{
			return_value = runbuiltin(d, &enode);
			exit(return_value);
		}
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
