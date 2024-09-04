/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:05:36 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/04 21:44:05 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	output_errors(t_list **error_list)
{
	while (*error_list)
	{
		ft_putendl_fd((*error_list)->content, STDERR_FILENO);
		*error_list = (*error_list)->next;
	}
	ft_lstclear(error_list, free);
}

static int	run_one(t_data *data, t_list *parse_nodes)
{
	t_exec_node	enode;
	int			fd_stdin;
	int			fd_stdout;
	int			exit_status;

	enode.parse = (t_parse_node *)parse_nodes->content;
	enode.pipes = malloc(sizeof(int *));
	enode.pipes[0] = NULL;
	enode.pindex = 0;
	enode.nofork = 1;
	enode.parse_nodes = parse_nodes;
	if (!check_fds(data, enode.parse->input_src, O_RDONLY)
		|| !check_fds(data, enode.parse->output_dest, O_CREAT | O_WRONLY))
		return (1);
	get_fds(data, &enode, enode.pipes);
	if (enode.fd_in == -1 || enode.fd_out == -1)
		return (1);
	fd_stdin = dup(STDIN_FILENO);
	fd_stdout = dup(STDOUT_FILENO);
	dup2(enode.fd_in, STDIN_FILENO);
	dup2(enode.fd_out, STDOUT_FILENO);
	exit_status = runbuiltin(data, &enode);
	dup2(fd_stdin, STDIN_FILENO);
	dup2(fd_stdout, STDOUT_FILENO);
	return (exit_status);
}

void	execution(t_data *data, t_list *parse_nodes)
{
	int		lsize;
	pid_t	*pids;

	lsize = ft_lstsize(parse_nodes);
	if (((t_parse_node *)parse_nodes->content)->is_builtin && lsize == 1)
		data->last_exit_code = run_one(data, parse_nodes);
	else
	{
		pids = fork_processes(data, parse_nodes, lsize);
		if (pids)
		{
			data->last_exit_code = waitpids(pids, lsize);
			free(pids);
		}
		else
			data->last_exit_code = 1;
	}
	output_errors(&data->error_list);
}
