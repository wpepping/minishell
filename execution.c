/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:05:36 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/29 20:36:58 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_one(t_data *data, t_list *parse_nodes)
{
	t_exec_node	enode;

	enode.parse = parse_nodes->content;
	enode.pipes = malloc(sizeof(int *));
	enode.pipes[0] = NULL;
	enode.pindex = 0;
	enode.nofork = 1;
	enode.parse_nodes = parse_nodes;
	get_fds(data, &enode, enode.pipes);
	if (enode.fd_in == -1 || enode.fd_out == -1)
		return (1);
	dup2(enode.fd_in, STDIN_FILENO);
	dup2(enode.fd_out, STDOUT_FILENO);
	return (runbuiltin(data, &enode));
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
		data->last_exit_code = waitpids(pids, lsize);
	}
}
