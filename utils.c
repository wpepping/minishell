/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:18:32 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/21 22:03:05 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_prompt(t_data *data)
{
	printf("%s%s ", data->cwd, PROMPT_END);
}

void	cleanup(t_data *data, t_exec_node *enode, t_parse_node *pnode)
{
	(void)data;
	(void)enode;
	(void)pnode;
}

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

char	*ft_pathjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (s1[len1 - 1] == '/')
		result = malloc((len1 + len2 + 1) * sizeof(char));
	else
		result = malloc((len1 + len2 + 2) * sizeof(char));
	if (result == NULL)
		return (NULL);
	ft_memcpy(result, s1, len1);
	if (s1[len1 - 1] != '/')
	{
		result[len1] = '/';
		len1++;
	}
	ft_memcpy(result + len1, s2, len2 + 1);
	return (result);
}
