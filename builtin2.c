/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:40:27 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/28 19:19:09 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_data *data, t_exec_node *node)
{
	char	*var;
	int		i;
	int		return_value;

	return_value = 0;
	i = 1;
	while (node->parse->argv[i])
	{
		var = ft_strdup(node->parse->argv[i]);
		if (var == NULL)
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			return_value = 1;
			break ;
		}
		else if (envp_set(&data->envp, var))
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			return_value = 1;
			free(var);
			break ;
		}
		i++;
	}
	return (return_value);
}

int	ft_unset(t_data *data, t_exec_node *node)
{
	char	**temp;
	int		return_value;

	return_value = 1;
	temp = envp_remove(data->envp, node->parse->argv + 1);
	if (temp != NULL)
	{
		free(data->envp);
		data->envp = temp;
		return_value = 0;
	}
	else
		ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
	return (return_value);
}

int	ft_env(t_data *data, t_exec_node *node)
{
	int	i;
	int	return_value;

	return_value = 1;
	i = 0;
	if (node->parse->argv[1] && node->parse->argv[1][0] == '-')
		invalid_option("env", node->parse->argv[1]);
	else
	{
		while (data->envp[i])
			ft_putendl_fd(data->envp[i++], STDOUT_FILENO);
		return_value = 0;
	}
	return (return_value);
}
