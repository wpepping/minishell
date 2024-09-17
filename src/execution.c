/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:27:58 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 16:19:34 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	output_errors(t_list **error_list)
{
	t_list	*node;

	node = *error_list;
	while (node)
	{
		ft_putendl_fd(node->content, STDERR_FILENO);
		node = node->next;
	}
	ft_lstclear(error_list, free);
}

static void	one_enode_init(t_data *data, t_exec_node *enode, t_list *pnodes)
{
	enode->parse = (t_parse_node *)pnodes->content;
	enode->pipes = create_pipes(0);
	if (!enode->pipes)
	{
		ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
		clean_exit(data, NULL, pnodes, 1);
	}
	enode->pindex = 0;
	enode->nofork = 1;
	enode->parse_nodes = pnodes;
	enode->run_cmd = true;
	enode->infile = NULL;
	enode->outfile = NULL;
	enode->fullcmd = NULL;
	enode->fd_in = -1;
	enode->fd_out = -1;
}

static int	run_one(t_data *data, t_list *parse_nodes)
{
	t_exec_node	enode;
	int			exit_status;

	one_enode_init(data, &enode, parse_nodes);
	if (!check_fds(data, enode.parse->redirect, &enode))
	{
		output_errors(&data->error_list);
		return (1);
	}
	get_fds(data, &enode, enode.pipes);
	if (enode.fd_in == -1 || enode.fd_out == -1)
		return (1);
	dup2(enode.fd_in, STDIN_FILENO);
	dup2(enode.fd_out, STDOUT_FILENO);
	exit_status = runbuiltin(data, &enode);
	dup2(data->fd_stdin, STDIN_FILENO);
	dup2(data->fd_stdout, STDOUT_FILENO);
	close_fds(enode.fd_in, enode.fd_out, enode.pipes);
	cleanup_run_one(&enode);
	return (exit_status);
}

static void	init_execution(t_data *data, t_execution *exec, t_list *pnodes)
{
	exec->pnodes = pnodes;
	exec->lsize = ft_lstsize(pnodes);
	exec->nofork = 0;
	exec->enodes = NULL;
	exec->pipes = create_pipes(exec->lsize - 1);
	if (!exec->pipes)
	{
		ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
		clean_exit(data, NULL, pnodes, 1);
	}
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
		cleanup_execution(&exec);
	}
}
