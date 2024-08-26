/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:40:27 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/26 18:28:04 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(t_data *data, t_exec_node *node)
{
	envp_set(&data->envp, node->parse->argv[1]);
}

void	ft_unset(t_data *data, t_exec_node *node)
{
	char	**temp;

	temp = envp_remove(data->envp, node->parse->argv[1]);
	free(data->envp);
	data->envp = temp;
	(void)data;
	(void)node;
}

void	ft_env(t_data *data, t_exec_node *node)
{
	int	i;

	if (node->parse->argv[1] && node->parse->argv[1][0] == '-')
		invalid_option("env", node->parse->argv[1]);
	else
	{
		while (data->envp[i])
			ft_putendl_fd(data->envp[i++], STDOUT_FILENO);
	}
}
