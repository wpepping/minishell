/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:43:25 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/23 19:01:24 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(int n)
{
	int	**pipes;
	int	i;

	pipes = malloc((n + 1) * sizeof(int *));
	pipes[n] = NULL;
	i = 0;
	while (i < n)
	{
		pipes[i] = malloc(2 * sizeof(int));
		pipe(pipes[i++]);
		//printf("pipes %i: %i %i\n", i - 1, pipes[i - 1][0], pipes[i - 1][1]);
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
			waitpid(pids[i], &temp, 0);
		}
		i++;
	}
	// need to update status with correct exit status
	return (status);
}
