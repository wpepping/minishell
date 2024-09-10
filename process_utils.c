/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:43:25 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/10 16:34:45 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(int n)
{
	int	**pipes;
	int	i;

	pipes = malloc((n + 1) * sizeof(int *));
	if (pipes == NULL)
		return (NULL);
	pipes[0] = NULL;
	i = 0;
	while (i < n)
	{
		pipes[i] = malloc(2 * sizeof(int));
		pipe(pipes[i++]);
		pipes[i] = NULL;
	}
	return (pipes);
}

int	waitpids(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	temp;

	i = 0;
	status = 0;
	while (i < n)
	{
		if (pids[i] != -1)
		{
			if (waitpid(pids[i], &temp, 0) == -1)
				status = 130;
			else if (WIFEXITED(temp))
				status = WEXITSTATUS(temp);
		}
		i++;
	}
	return (status);
}

int	oflags(t_token_type type)
{
	if (type == REDIRECT_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	if (type == APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	return (O_RDONLY);
}
