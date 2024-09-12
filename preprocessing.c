/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:59:44 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/11 16:58:20 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cleanup_prep(int **pipes, t_list *lst, t_exec_node *enode)
{
	free(pipes);
	ft_lstclear(&lst, free);
	free(enode);
	return (1);
}

static t_exec_node	*init_exec_node(t_parse_node *pnode, t_execution *exec)
{
	t_exec_node	*enode;

	enode = malloc(sizeof(t_exec_node));
	if (!enode)
		return (NULL);
	enode->nofork = exec->nofork;
	enode->list_size = exec->lsize;
	enode->parse_nodes = exec->pnodes;
	enode->parse = pnode;
	enode->pipes = exec->pipes;
	enode->error_code = 1;
	enode->infile = NULL;
	enode->outfile = NULL;
	enode->fullcmd = NULL;
	return (enode);
}

t_list	*create_exec_nodes(t_data *data, t_execution *exec)
{
	t_exec_node		*enode;
	int				i;
	t_list			*lst;
	t_list			*node;

	i = 0;
	lst = exec->pnodes;
	while (lst)
	{
		enode = init_exec_node(lst->content, exec);
		node = ft_lstnew(enode);
		if (!enode || !node)
			exit(cleanup_prep(exec->pipes, lst, enode));
		ft_lstadd_back(&exec->enodes, node);
		enode->run_cmd = !enode->parse->heredoc_fail
			&& check_fds(data, enode->parse->redirect, enode);
		enode->run_cmd = enode->run_cmd && check_cmd(data, enode);
		enode->pindex = i++;
		lst = lst->next;
	}
	return (exec->enodes);
}
