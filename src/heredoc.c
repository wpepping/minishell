/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:12:04 by phartman          #+#    #+#             */
/*   Updated: 2024/11/13 17:44:37 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	process_line(char *line, char *delimiter, t_data data, int fd);
static int	read_heredoc(char *filename, char *delimiter, t_data data);
static int	fork_heredoc(char *filename, char *delimiter, t_data data);

static void	switch_signal_handler(int signal, __sighandler_t handler)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, signal);
	sa.sa_flags = 0;
	sigaction(signal, &sa, NULL);
}

int	handle_heredoc(char *delimiter, t_data data, t_token *token)
{
	char	*filename;
	int		return_val;

	return_val = 0;
	filename = (char *)malloc(13 * sizeof(char));
	get_tempfile_name(filename);
	token->value = filename;
	token->type = HEREDOC;
	return_val = fork_heredoc(filename, delimiter, data);
	if (access(filename, F_OK) == 0 && return_val == 130)
		unlink(filename);
	free(delimiter);
	return (return_val);
}

static int	process_line(char *line, char *delimiter, t_data data, int fd)
{
	t_token	*token;

	if (!line || (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\0'))
	{
		free(line);
		return (0);
	}
	token = malloc(sizeof(t_token));
	malloc_protection(token);
	token->value = line;
	if (ft_strchr(line, '$'))
		expand_env(token, ft_strchr(line, '$'), data);
	write(fd, token->value, ft_strlen(token->value));
	write(fd, "\n", 1);
	free(token->value);
	free(token);
	return (1);
}

static int	read_heredoc(char *filename, char *delimiter, t_data data)
{
	int					fd;
	int					continue_reading;

	switch_signal_handler(SIGINT, SIG_DFL);
	switch_signal_handler(SIGQUIT, SIG_IGN);
	continue_reading = 1;
	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (1);
	while (continue_reading)
		continue_reading = process_line(readline("> "), delimiter, data, fd);
	close(fd);
	exit(0);
}

static int	fork_heredoc(char *filename, char *delimiter, t_data data)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	pid = fork();
	if (pid < 0)
		return (ft_puterr("failed to create process: ", NULL, NULL), 1);
	else if (pid == 0)
		return (read_heredoc(filename, delimiter, data));
	else
	{
		switch_signal_handler(SIGQUIT, SIG_IGN);
		if (waitpid(pid, &status, 0) == -1)
		{
			ft_puterr("failed to wait for heredoc process", NULL, NULL);
			exit_status = 1;
		}
		if (WIFEXITED(status))
			exit_status = 0;
		else
			exit_status = WTERMSIG(status) + 128;
		switch_signal_handler(SIGQUIT, sigquit_handler);
		return (exit_status);
	}
}
