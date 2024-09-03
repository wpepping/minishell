/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:40:27 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/29 16:03:41 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_varname(char *name)
{
	int	i;

	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	i = 0;
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
			ft_putstrs_fd("minishell: export: '", var,
				"': not a valid identifier", STDERR_FILENO);
			return_value = 1;
			continue ;
		}
		if (!ft_strchr(var, '='))
			continue ;
		var = ft_strdup(var);
		if (var == NULL || envp_set(&data->envp, var))
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			return_value = 1;
			free(var);
			break ;
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
