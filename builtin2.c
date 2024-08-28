/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:40:27 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/28 15:57:58 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(t_data *data, t_exec_node *node)
{
	char	*var;
	int		i;

	i = 1;
	while (node->parse->argv[i])
	{
		var = ft_strdup(node->parse->argv[i]);
		if (var == NULL)
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			break ;
		}
		else if (envp_set(&data->envp, var))
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			free(var);
			break ;
		}
		i++;
	}
}

void	ft_unset(t_data *data, t_exec_node *node)
{
	char	**temp;

	temp = envp_remove(data->envp, node->parse->argv + 1);
	if (temp != NULL)
	{
		free(data->envp);
		data->envp = temp;
	}
	else
		ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
}

void	ft_env(t_data *data, t_exec_node *node)
{
	int	i;

	i = 0;
	if (node->parse->argv[1] && node->parse->argv[1][0] == '-')
		invalid_option("env", node->parse->argv[1]);
	else
	{
		while (data->envp[i])
			ft_putendl_fd(data->envp[i++], STDOUT_FILENO);
	}
}
