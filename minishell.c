/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:11:46 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 13:14:24 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data, char **envp)
{
	data->node_list = NULL;
	data->exit = 0;
	data->envp = envp_create(envp);
	data->last_exit_code = 0;
	data->error_list = NULL;
	data->fd_stdin = dup(STDIN_FILENO);
	data->fd_stdout = dup(STDOUT_FILENO);
	if (!data->envp)
	{
		ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
		cleanup(data);
		exit(1);
	}
	getcwd(data->cwd, PATH_MAX);
}

static void	cmd_handl(t_data *data, char *cmd, t_sigact sa_int)
{
	add_history(cmd);
	sa_int.sa_handler = process_running_sigint_handler;
	sigaction(SIGINT, &sa_int, NULL);
	if (!parse(data, cmd) && data->node_list)
		execution(data, data->node_list);
	else if(data->node_list)
		free_parse_node(data->node_list);
	sa_int.sa_handler = default_sigint_handler;
	sigaction(SIGINT, &sa_int, NULL);
	clean_heredocs(data->node_list);
	cleanup_cmd(cmd, data->node_list);
	data->node_list = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_data		data;
	char		*cmd;
	char		*prompt;
	t_sigaction	sa_int;
	t_sigaction	sa_quit;

	(void)argc;
	(void)argv;
	init_signal_handlers(&sa_int, &sa_quit);
	init(&data, envp);
	while (!data.exit)
	{
		prompt = ft_strjoin(data.cwd, PROMPT_END);
		cmd = readline(prompt);
		free(prompt);
		if (!cmd)
			data.exit = 1;
		else if (*cmd != '\0')
			cmd_handl(&data, cmd, sa_int);
		else
			free(cmd);
	}
	cleanup(&data);
	return (data.last_exit_code);
}
