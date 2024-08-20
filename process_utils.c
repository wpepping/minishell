/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:43:25 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/20 18:43:46 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**create_pipes(int n)
{
	int	**pfds;
	int	i;

	pfds = malloc((n + 1) * sizeof(int *));
	pfds[n] = NULL;
	i = 0;
	while (i < n)
	{
		pfds[i] = malloc(2 * sizeof(int));
		pipe(pfds[i++]);
	}
	return (pfds);
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
			if (WIFEXITED(temp))
				status = max(status, WEXITSTATUS(temp));
		}
		i++;
	}
	return (status);
}
