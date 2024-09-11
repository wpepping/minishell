/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:14:03 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/11 17:23:00 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin2(char *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc((len1 + len2 + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2 + 1);
	free(s1);
	return (result);
}

void	ft_puterr(char *str1, char *str2, char *str3)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (str1 != NULL)
		ft_putstr_fd(str1, STDERR_FILENO);
	if (str2 != NULL)
		ft_putstr_fd(str2, STDERR_FILENO);
	if (str3 != NULL)
		ft_putstr_fd(str3, STDERR_FILENO);
	ft_putendl_fd("", STDERR_FILENO);
}

void	clean_exit(t_data *d, t_exec_node *enode, t_list *pnodes, int status)
{
	cleanup(d);
	if (enode)
	{
		close_fds(enode->fd_in, enode->fd_out, enode->pipes);
		free_array((void **)enode->pipes);
	}
	if (pnodes)
		cleanup_cmd(NULL, pnodes);
	exit(status);
}

bool	isdir(char *dname)
{
	DIR	*dir;

	dir = opendir(dname);
	if (!dir)
		return (false);
	closedir(dir);
	return (true);
}

int	ft_isint(char *str)
{
	long	l;
	char	*p;

	p = str;
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0' || ft_strlen(str) > 10)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	l = ft_atol(p);
	if (l > 2147483647 || l < -2147483648)
		return (0);
	return (1);
}
