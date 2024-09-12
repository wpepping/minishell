/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:12:04 by phartman          #+#    #+#             */
/*   Updated: 2024/09/12 16:13:51 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_line(char *line, char *delimiter, t_data data, int fd);
static int	read_heredoc(char *filename, char *delimiter, t_data data);
static int	fork_heredoc(char *filename, char *delimiter, t_data data);

int	handle_heredoc(char *delimiter, t_data data, t_token *token)
{
	char	*filename;
	int		return_val;

	return_val = 0;
	filename = (char *)malloc(12 * sizeof(char));
	get_tempfile_name(filename);
	token->value = filename;
	token->type = HEREDOC;
	return_val = fork_heredoc(filename, delimiter, data);
	free(delimiter);
	return (return_val);
}

static int	process_line(char *line, char *delimiter, t_data data, int fd)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!line || (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\0'))
	{
		free(line);
		return (0);
	}
	token->value = line;
	if (ft_strchr(line, '$'))
	{
		expand_env(token, ft_strchr(line, '$'), data);
		free(line);
	}
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
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
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

	pid = fork();
	if (pid < 0)
		return (ft_puterr("failed to create process: ", NULL, NULL), 1);
	else if (pid == 0)
		return (read_heredoc(filename, delimiter, data));
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			return (ft_puterr("failed to wait for heredoc process", NULL, NULL),
				1);
		if (WIFEXITED(status))
			return (0);
		else
			return (WTERMSIG(status) + 128);
	}
}
