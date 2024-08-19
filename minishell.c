/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:24 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/18 19:44:26 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data)
{
	data->exit = 0;
	getcwd(data->cwd, PATH_MAX);
}

int	main(void)
{
	t_data	data;
	char	*cmd;

	init(&data);
	while (!data.exit)
	{
		print_prompt(&data);
		cmd = readline(NULL);
		parse(&data, cmd);
		free(cmd);
	}
	return (0);
}
