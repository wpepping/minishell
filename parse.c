/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:27:25 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 17:28:44 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_data *data, char *cmd)
{
	int	strlen;

	strlen = ft_strlen(cmd);
	if (cmd == NULL || ft_strncmp(cmd, "exit", 5) == 0)
	{
		data->exit = 1;
	}
	else if (ft_strncmp(cmd, "cd ", 3) == 0 || ft_strncmp(cmd, "cd", 3) == 0)
		cd(data, cmd);
	else if (ft_strncmp(cmd, "test", 5) == 0)
		run_execution_test(data);
	else if (strlen > 0)
		printf("minishell: command not found: %s\n", cmd);
	else
		printf("\n");
}
