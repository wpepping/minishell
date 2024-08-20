/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:31 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/20 19:08:14 by phartman         ###   ########.fr       */
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
#include <stdbool.h>

# define PROMPT_END "$"

typedef struct s_data
{
	char	cwd[PATH_MAX];
	int		exit;
}	t_data;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_parse_node
{
	bool	is_builtin;
	bool 	append;
	bool 	heredoc;
	char	*exec;
	char 	**argv;
	char 	**output_dest;
	char 	**input_src;
	
}	t_parse_node;


const char *builtin_str[] = {
    "echo",
	"cd",
	"unset",
	"export",
	
	"pwd",
	"env",
	"exit"
};

void	parse(t_data *data, char *cmd);
void	print_prompt(t_data *data);
t_parse_node	*create_parse_node(void);
int	get_builtin_index(char *token);
int	get_args(int index, char **tokens, t_parse_node *node);
//char *find_executable_path(char *token);

// Builtins
void	cd(t_data *data, char *cmd);
void	echo(char *cmd);


#endif
