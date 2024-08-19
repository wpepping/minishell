/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/18 20:15:14 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_data *data, char *cmd)
{
	int	strlen;

	strlen = ft_strlen(cmd);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		data->exit = 1;
	else if (ft_strncmp(cmd, "cd ", 3) == 0)
		cd(data, cmd);
	else if (strlen > 0)
		printf("minishell: command not found: %s\n", cmd);
	else
		printf("\n");
}
