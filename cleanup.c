/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 18:29:04 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/09 18:45:34 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirect(void *node)
{
	t_token	*token;

	token = (t_token *)node;
	free(token->value);
	free(node);
}

static void	free_exec_node(void *node)
{
	t_exec_node	*enode;

	enode = (t_exec_node *)node;
	if (enode->fullcmd)
		free(enode->fullcmd);
	free(node);
}

static void	free_parse_node(void *node)
{
	t_parse_node	*pnode;

	pnode = (t_parse_node *)node;
	free_array((void **)pnode->argv);
	ft_lstclear(&pnode->redirect, free_redirect);
	free(node);
}

void	cleanup_cmd(char *cmd, t_list *pnode)
{
	if (cmd)
		free(cmd);
	ft_lstclear(&pnode, &free_parse_node);
}

void	cleanup_execution(t_execution *exec)
{
	ft_lstclear(&exec->enodes, &free_exec_node);
	free_array((void **)exec->pipes);
}
