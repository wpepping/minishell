/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:05:36 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/21 22:05:29 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution(t_data *data, t_list *parse_nodes)
{
	int		lsize;
	pid_t	*pids;

	lsize = ft_lstsize(parse_nodes);
	pids = fork_processes(data, parse_nodes, lsize);
	waitpids(pids, lsize);
}

void	run_execution_test(t_data *data)
{
	t_parse_node	p0;
	t_parse_node	p1;
	t_parse_node	p2;
	t_list			*lst;

	lst = NULL;

	p0.argv = malloc(3 * sizeof(char *));
	p0.argv[0] = ft_strdup("rm");
	p0.argv[1] = ft_strdup("a");
	p0.argv[2] = NULL;
	p0.is_builtin = 0;
	p0.here_doc = 0;
	p0.is_first = 1;
	p0.is_last = 0;
	p0.exec = NULL;
	p0.input_src = NULL;
	p0.output_dest = NULL;

	p1.argv = malloc(2 * sizeof(char *));
	p1.argv[0] = ft_strdup("ls");
	p1.argv[1] = ft_strdup("-l");
	p1.argv[2] = NULL;
	p1.is_builtin = 0;
	p1.here_doc = 0;
	p1.is_first = 0;
	p1.is_last = 0;
	p1.exec = NULL;
	p1.input_src = NULL;
	p1.output_dest = NULL;

	p2.argv = malloc(3 * sizeof(char *));
	p2.argv[0] = ft_strdup("wc");
	p2.argv[1] = ft_strdup("-l");
	p2.argv[2] = NULL;
	p2.is_builtin = 0;
	p2.here_doc = 0;
	p2.is_first = 0;
	p2.is_last = 1;
	p2.exec = NULL;
	p2.input_src = NULL;
	p2.output_dest = NULL;

	ft_lstadd_front(&lst, ft_lstnew(&p2));
	ft_lstadd_front(&lst, ft_lstnew(&p1));
	ft_lstadd_front(&lst, ft_lstnew(&p0));
	execution(data, lst);
}
