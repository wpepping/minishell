/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:40:27 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/12 20:16:59 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		{
			if (ft_strchr(data->envp[i], '='))
				ft_putendl_fd(data->envp[i], STDOUT_FILENO);
			i++;
		}
		return_value = 0;
	}
	return (return_value);
}

int	ft_exit(t_data *data, t_exec_node *node)
{
	int	exit_code;
	int	val;

	exit_code = 0;
	if (node->parse->argv[1])
	{
		val = ft_atoi(node->parse->argv[1]);
		exit_code = val % 256;
		if (exit_code < 0)
			exit_code += 256;
		if (val == 0)
		{
			ft_puterr("exit: ", node->parse->argv[1],
				": numeric argument required");
			exit_code = 2;
		}
		else if (node->parse->argv[2])
		{
			ft_puterr(ERR_EXIT_TOO_MANY_ARG, NULL, NULL);
			exit_code = 1;
		}
	}
	data->exit = 1;
	return (exit_code);
}
