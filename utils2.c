/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:14:03 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/04 20:04:06 by phartman         ###   ########.fr       */
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

void	ft_putstrs_fd(char *str1, char *str2, char *str3, int fd)
{
	if (str1 != NULL)
		ft_putstr_fd(str1, fd);
	if (str2 != NULL)
		ft_putstr_fd(str2, fd);
	if (str3 != NULL)
		ft_putstr_fd(str3, fd);
	ft_putendl_fd("", fd);
}

void	clean_exit(t_data *data, t_exec_node *enode, t_list *parse_nodes)
{
	(void)data;
	(void)enode;
	(void)parse_nodes;
	exit(1);
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
