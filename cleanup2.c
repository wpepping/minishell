/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 17:54:13 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/09 18:07:02 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_data *data, t_exec_node *enode, t_parse_node *pnode)
{
	free_array((void **)data->envp);
	clear_history();
	(void)enode;
	(void)pnode;
}

void	cleanup_run_one(void *node)
{
	t_exec_node	*enode;

	enode = (t_exec_node *)node;
	if (enode->fullcmd)
		free(enode->fullcmd);
	free_array((void **)enode->pipes);
}
