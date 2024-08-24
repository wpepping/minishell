/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:05:36 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/24 18:36:31 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_one(t_data *data, t_list *parse_nodes)
{
	t_exec_node	enode;

	enode.parse = parse_nodes->content;
	enode.pipes = malloc(sizeof(int *));
	enode.pipes[0] = NULL;
	enode.pindex = 0;
	enode.fd_in = 0;
	enode.fd_out = 1;
	runbuiltin(data, &enode);
}

void	execution(t_data *data, t_list *parse_nodes)
{
	int		lsize;
	pid_t	*pids;

	lsize = ft_lstsize(parse_nodes);
	if (((t_parse_node *)parse_nodes->content)->is_builtin && lsize == 1)
		run_one(data, parse_nodes);
	else
	{
		pids = fork_processes(data, parse_nodes, lsize);
		waitpids(pids, lsize);
	}
}
