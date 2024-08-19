/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:31 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/19 18:24:04 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <linux/limits.h>
# include <readline/readline.h>
# include "libft/libft.h"
# include <signal.h>
# include <bits/sigaction.h>

# define PROMPT_END "$"

typedef struct s_data
{
	char	cwd[PATH_MAX];
	int		exit;
}	t_data;

void	parse(t_data *data, char *cmd);
void	print_prompt(t_data *data);

// Builtins
void	cd(t_data *data, char *cmd);
void	echo(char *cmd);

#endif
