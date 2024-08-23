/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:24 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/23 15:38:41 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data)
{
	data->node_list = NULL;
	data->exit = 0;
	getcwd(data->cwd, PATH_MAX);
}

void	signal_handler(int signum)
{
	(void)signum;
	printf("\ninterrupted\n");
}

int	main(void)
{
	t_data				data;
	char				*cmd;
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = signal_handler;
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	init(&data);
	while (!data.exit)
	{
		print_prompt(&data);
		cmd = readline(NULL);

		parse(&data, cmd);
		free(cmd);
	}
	return (0);
}
