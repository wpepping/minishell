/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:36:57 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/12 20:29:40 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
