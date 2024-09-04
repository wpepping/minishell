/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:23:32 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/05 15:53:06 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_cd_getpath(t_data *data, char	*arg)
{
	char	*path;

	if (arg[0] == '/')
		path = ft_strdup("");
	else
		path = ft_strjoin(data->cwd, "/");
	path = ft_strjoin2(path, arg);
	return (path);
}

int	ft_echo(t_data *data, t_exec_node *node)
{
	int	i;
	int	arg_no_newln;

	(void)data;
	if (node->parse->argv[1] && ft_strncmp(node->parse->argv[1], "-n", 3) == 0)
		arg_no_newln = 1;
	else
		arg_no_newln = 0;
	i = 1;
	if (arg_no_newln)
		i++;
	while (node->parse->argv[i])
	{
		ft_putstr_fd(node->parse->argv[i++], STDOUT_FILENO);
		if (node->parse->argv[i])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (!arg_no_newln)
		ft_putendl_fd("", STDOUT_FILENO);
	return (0);
}

int	ft_cd(t_data *data, t_exec_node *node)
{
	char	*path;
	int		return_value;
	int		errnr;

	return_value = 1;
	if (node->parse->argv[2])
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
	else if (node->parse->argv[1] && node->parse->argv[1][0] == '-')
		invalid_option("cd", node->parse->argv[1]);
	else if (node->parse->argv[1])
	{
		path = ft_cd_getpath(data, node->parse->argv[1]);
		errnr = chdir(path);
		if (errnr == EACCES)
			ft_putstrs_fd("minishell: cd: ", node->parse->argv[1],
				": Permission denied", STDERR_FILENO);
		else if (errnr)
			ft_putstrs_fd("minishell: cd: ", node->parse->argv[1],
				": No such file or directory", STDERR_FILENO);
		else
			return_value = 0;
		getcwd(data->cwd, PATH_MAX);
		free(path);
	}
	return (return_value);
}

int	ft_pwd(t_data *data, t_exec_node *node)
{
	if (node->parse->argv[1] && node->parse->argv[1][0] == '-'
		&& ft_strlen(node->parse->argv[1]) > 1)
	{
		invalid_option("pwd", node->parse->argv[1]);
		return (1);
	}
	ft_putendl_fd(data->cwd, 1);
	return (0);
}

int	ft_exit(t_data *data, t_exec_node *node)
{
	int exit_code;
	int val;

	exit_code = 0;
	
	if(node->parse->argv[1])
	{
		val = ft_atoi(node->parse->argv[1]);
		if(val == 0 && node->parse->argv[1][0] != 0)
		{
			printf("numeric argument required\n");
			exit_code = 2;
		}
		if(node->parse->argv[2])
		{
			printf(" too many arguments\n");
			exit_code = 1;
		}
		else
		{
			exit_code = val % 256;
			if(exit_code < 0)
				exit_code += 256;
		}
	}
	data->exit = exit_code;
	return (exit_code);
}
