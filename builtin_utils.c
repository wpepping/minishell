/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:18:32 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/26 18:29:46 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	invalid_option(char *command, char *option)
{
	char	*o;

	if (ft_strlen(option) == 1)
		ft_putstrs_fd("minishell: ", command, ": invalid option", STDERR_FILENO);
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
