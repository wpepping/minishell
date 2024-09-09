/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:18:32 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/09 18:44:37 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	invalid_option(char *command, char *option)
{
	char	*o;

	if (ft_strlen(option) == 1)
		ft_puterr(command, ": invalid option", NULL);
	else
	{
		o = ft_substr(option, 0, 2);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(o, STDERR_FILENO);
		ft_putendl_fd(": invalid option", STDERR_FILENO);
		free(o);
	}
}

void	malloc_protection(void *ptr)
{
	if (!ptr)
	{
		ft_puterr("Error: malloc failed", NULL, NULL);
		exit(1);
	}
}


