/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:29:14 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 19:07:31 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

static int	export_print_line(char *str)
{
	char	*pos;
	char	*varname;

	ft_putstr_fd(DECLARE_X, STDOUT_FILENO);
	pos = ft_strchr(str, '=');
	if (pos)
	{
		varname = ft_substr(str, 0, pos - str + 1);
		if (!varname)
			return (-1);
		ft_putstr_fd(varname, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
		ft_putstr_fd(pos + 1, STDOUT_FILENO);
		ft_putendl_fd("\"", STDOUT_FILENO);
		free(varname);
	}
	else
		ft_putendl_fd(str, STDOUT_FILENO);
	return (0);
}

static int	export_list(t_data *d)
{
	int		i;
	char	*st;
	char	*prev;

	prev = NULL;
	while (1)
	{
		i = -1;
		st = NULL;
		while (d->envp[++i])
			if ((!prev || ft_strncmp(d->envp[i], prev, ft_strlen(prev) + 1) > 0)
				&& (!st || ft_strncmp(d->envp[i], st, ft_strlen(st) + 1) < 0))
				st = d->envp[i];
		if (!st)
			break ;
		if (export_print_line(st) == -1)
		{
			ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
			return (1);
		}
		prev = st;
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
