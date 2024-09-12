/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 17:54:13 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/11 20:11:00 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_heredocs(t_list *node)
{
	t_list			*redirect_node;
	t_parse_node	*pnode;
	t_token			*token;

	while (node)
	{
		pnode = node->content;
		redirect_node = pnode->redirect;
		while (redirect_node)
		{
			token = (t_token *)redirect_node->content;
			if (token->type == HEREDOC && access(token->value, F_OK) == 0)
				unlink(token->value);
			redirect_node = redirect_node->next;
		}
		node = node->next;
	}
}

void	cleanup(t_data *data)
{
	free_array((void **)data->envp);
	close(data->fd_stdin);
	close(data->fd_stdout);
	clear_history();
}

void	cleanup_run_one(void *node)
{
	t_exec_node	*enode;

	enode = (t_exec_node *)node;
	if (enode->fullcmd)
		free(enode->fullcmd);
	free_array((void **)enode->pipes);
}
