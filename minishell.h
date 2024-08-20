/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:31 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/20 18:41:44 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <linux/limits.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include "libft/libft.h"

# define ERR_COMMAND_NOT_FOUND "minishell: command not found: "
# define ERR_OUT_OF_MEMORY "minishell: out of memory error"

# define PROMPT_END "$"

typedef struct s_exec_node
{
	t_parse_node	*parse;
	int				fd_in;
	int				fd_out;
	int				**pipes;
	bool			run_cmd;
}	t_exec_node;

typedef struct s_parse_node
{
	bool	is_builtin;
	bool	here_doc;
	char	*exec;
	char	**argv;
	char	**output_dest;
	char	**input_src;
}	t_parse_node;

typedef struct s_data
{
	char	cwd[PATH_MAX];
	char	**path;
	t_list	*envp;
	int		exit;
}	t_data;

void	parse(t_data *data, char *cmd);
void	print_prompt(t_data *data);

// Builtins
void	cd(t_data *data, char *cmd);

#endif
