/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:40:27 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/11 18:31:42 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_varname(char *name)
{
	int	i;

	if (!isalnum(name[0]) && name[0] != '_')
		return (0);
	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_data *data, t_exec_node *node)
{
	char	*var;
	int		i;
	int		return_value;

	return_value = 0;
	i = 1;
	while (node->parse->argv[i])
	{
		var = node->parse->argv[i++];
		if (!is_valid_varname(var))
		{
			ft_puterr("export: '", var, "': not a valid identifier");
			return_value = 1;
			continue ;
		}
		if (!ft_strchr(var, '='))
			continue ;
		var = ft_strdup(var);
		if (var == NULL || envp_set(&data->envp, var))
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			return (free(var), 1);
		}
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
