/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:57 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 17:13:31 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strchr2(char *str, char c)
{
	char	*pos;

	pos = ft_strchr(str, c);
	if (!pos)
		pos = ft_strchr(str, '\0');
	return (pos);
}

int	ft_envncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i += 1;
	}
	if (i == n)
		return (0);
	if ((s1[i] == '\0' && s2[i] == '=') || (s1[i] == '=' && s2[i] == '\0'))
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

bool	is_valid_exit_code(char *nptr)
{
	char	*temp;

	temp = nptr;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	while (ft_isdigit(*nptr))
		nptr++;
	if (*nptr != '\0')
		return (false);
	return (ft_islong(temp));
}
