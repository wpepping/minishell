/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 18:29:04 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/09 16:03:56 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_parse_node(void *node)
{
	t_parse_node	*pnode;

	pnode = (t_parse_node *)node;
	free_array((void **)pnode->argv);
	ft_lstclear(&pnode->redirect, free);
	//ft_lstclear(&pnode->output_dest, free);
	//ft_lstclear(&pnode->input_src, free);
}

void	cleanup(t_data *data, t_exec_node *enode, t_parse_node *pnode)
{
	free_array((void **)data->envp);
	clear_history();
	(void)enode;
	(void)pnode;
}

void	cleanup_cmd(t_list *pnode)
{
	ft_lstclear(&pnode, &free_parse_node);
}
