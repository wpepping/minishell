/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:18:32 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/09 18:51:55 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(void **arr)
{
	int	i;

	if (arr != NULL)
	{
		i = 0;
		while (arr[i] != NULL)
			free(arr[i++]);
		free(arr);
	}
}

void	close_fds(int fd_in, int fd_out, int **pipes)
{
	int	i;

	if (fd_in != -1 && fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != -1 && fd_out != STDOUT_FILENO)
		close(fd_out);
	i = 0;
	while (pipes[i] != NULL)
	{
		if (pipes[i][0] != -1)
			close(pipes[i][0]);
		if (pipes[i][1] != -1)
			close(pipes[i][1]);
		i++;
	}
}

int	arrncontains(char **haystack, char *needle, int cmplen)
{
	while (*haystack)
	{
		if (ft_strncmp(*haystack, needle, cmplen) == 0)
			return (1);
		haystack++;
	}
	return (0);
}


t_parse_node	*create_parse_node(void)
{
	t_parse_node	*node;

	node = malloc(sizeof(t_parse_node));
	malloc_protection(node);
	node->is_last = false;
	node->is_builtin = false;
	node->argv = NULL;
	node->redirect = NULL;
	return (node);
}

