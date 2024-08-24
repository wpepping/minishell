/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:24 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/24 13:15:57 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data, char **envp)
{
	data->exit = 0;
	data->envp = envp_create(envp);
	getcwd(data->cwd, PATH_MAX);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char	*cmd;

	(void)argc;
	(void)argv;
	init(&data, envp);
	while (!data.exit)
	{
		print_prompt(&data);
		cmd = readline(NULL);
		parse(&data, cmd);
		free(cmd);
	}
	return (0);
}
