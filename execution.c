/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:05:36 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/08 17:41:14 by wpepping         ###   ########.fr       */
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

static void	one_enode_init(t_exec_node *enode, t_list *parse_nodes)
{
	enode->parse = (t_parse_node *)parse_nodes->content;
	enode->pipes = create_pipes(0);
	enode->pindex = 0;
	enode->nofork = 1;
	enode->parse_nodes = parse_nodes;
	enode->run_cmd = true;
	enode->infile = NULL;
	enode->outfile = NULL;
}

static int	run_one(t_data *data, t_list *parse_nodes)
{
	t_exec_node	enode;
	int			fd_stdin;
	int			fd_stdout;
	int			exit_status;

	one_enode_init(&enode, parse_nodes);
	if (!check_fds(data, enode.parse->input_src, &enode)
		|| !check_fds(data, enode.parse->output_dest, &enode))
	{
		output_errors(&data->error_list);
		return (1);
	}
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

static void	init_execution(t_data *data, t_execution *exec, t_list *pnodes)
{
	exec->pnodes = pnodes;
	exec->lsize = ft_lstsize(pnodes);
	exec->nofork = 0;
	exec->enodes = NULL;
	exec->pipes = create_pipes(exec->lsize - 1); // Deal with NULL
	exec->enodes = create_exec_nodes(data, exec);
	output_errors(&data->error_list);
}

void	execution(t_data *data, t_list *pnodes)
{
	pid_t		*pids;
	t_execution	exec;

	if (((t_parse_node *)pnodes->content)->is_builtin && !pnodes->next)
		data->last_exit_code = run_one(data, pnodes);
	else
	{
		init_execution(data, &exec, pnodes);
		pids = fork_processes(data, &exec);
		if (pids)
		{
			data->last_exit_code = waitpids(pids, exec.lsize);
			free(pids);
		}
		else
			data->last_exit_code = 1;
	}
}
