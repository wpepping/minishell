/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:29:14 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/12 20:52:46 by wpepping         ###   ########.fr       */
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

static int	export_list(t_data *data)
{
	int		i;
	char	*str;
	char	*prev;

	prev = NULL;
	while (1)
	{
		i = 0;
		str = NULL;
		while (data->envp[i])
		{
			if ((!prev || strncmp(data->envp[i], prev, ft_strlen(prev) + 1) > 0)
				&& (!str || strncmp(data->envp[i], str, ft_strlen(str) + 1) < 0))
				str = data->envp[i];
			i++;
		}
		if (!str)
			break ;
		ft_putendl_fd(str, STDOUT_FILENO);
		prev = str;
	}
	return (0);
}

int	ft_export(t_data *data, t_exec_node *node)
{
	char	*var;
	int		i;
	int		return_value;

	return_value = 0;
	i = 1;
	if (node->parse->argv[1] == NULL)
		return (export_list(data));
	while (node->parse->argv[i])
	{
		var = node->parse->argv[i++];
		if (!is_valid_varname(var))
		{
			ft_puterr("export: '", var, "': not a valid identifier");
			return_value = 1;
			continue ;
		}
		var = ft_strdup(var);
		if (var == NULL || envp_set(&data->envp, var))
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			return (free(var), 1);
		}
	}
	return (return_value);
}
