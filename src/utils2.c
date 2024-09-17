/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:14:03 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 17:28:16 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

bool	ft_islong(char *str)
{
	unsigned long	l;
	char			*p;
	bool			negative;

	while (*str == ' ' || *str == '\f' || *str == '\n'
		|| *str == '\r' || *str == '\t' || *str == '\v')
		str++;
	if (*str == '-')
		negative = true;
	if (*str == '-' || *str == '+')
		str++;
	p = str;
	if (*str == '\0' || ft_strlen(str) > 19)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	l = ft_atoul(p);
	if ((!negative && l > (unsigned long)LLONG_MAX)
		|| (negative && l > (unsigned long)LLONG_MAX + 1UL))
		return (false);
	return (true);
}
