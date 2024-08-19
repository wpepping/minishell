/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/19 18:30:44 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_data *data, char *cmd)
{
	int	strlen;

	if (cmd == NULL || ft_strncmp(cmd, "exit", 5) == 0)
	{
		data->exit = 1;
		return ;
	}
	strlen = ft_strlen(cmd);
	if (ft_strncmp(cmd, "cd ", 3) == 0)
		cd(data, cmd);
	else if (strlen > 0)
		printf("minishell: command not found: %s\n", cmd);
	else
		printf("\n");
}
