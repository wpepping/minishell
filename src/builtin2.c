/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:17:39 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/20 16:24:58 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_unset(t_data *data, t_exec_node *node)
{
	char	**temp;

	temp = envp_remove(data->envp, node->parse->argv + 1);
	if (temp == NULL)
	{
		ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
		return (1);
	}
	free(data->envp);
	data->envp = temp;
	return (0);
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
	if (node->nofork)
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (node->parse->argv[1])
	{
		val = ft_atol(node->parse->argv[1]);
		exit_code = val % 256;
		if (exit_code < 0)
			exit_code += 256;
		if (node->parse->argv[2])
		{
			ft_puterr(ERR_EXIT_TOO_MANY_ARG, NULL, NULL);
			return (1);
		}
		else if (!is_valid_exit_code(node->parse->argv[1]))
		{
			ft_puterr("exit: ", node->parse->argv[1], ERR_NUMERIC_ARG);
			exit_code = 2;
		}
	}
	data->exit = 1;
	return (exit_code);
}
