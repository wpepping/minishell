/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:25:47 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 17:28:21 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data, char **envp)
{
	data->exit = 0;
	data->envp = envp_create(envp);
	getcwd(data->cwd, PATH_MAX);
}

void	signal_handler(int signum)
{
	(void)signum;
	printf("\ninterrupted\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_data				data;
	char				*cmd;
	char				*prompt;
	struct sigaction	sa;

	(void)argc;
	(void)argv;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = signal_handler;
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	init(&data, envp);
	while (!data.exit)
	{
		prompt = ft_strjoin(data.cwd, PROMPT_END);
		cmd = readline(prompt);
		free(prompt);
		parse(&data, cmd);
		free(cmd);
	}
	return (0);
}
