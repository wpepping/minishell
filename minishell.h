/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:31 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/23 16:16:37 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include <linux/limits.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include "libft/libft.h"

# define ERR_COMMAND_NOT_FOUND "minishell: command not found: "
# define ERR_OUT_OF_MEMORY "minishell: out of memory error"

# define PROMPT_END "$"

typedef struct s_parse_node
{
	bool	is_builtin;
	bool	here_doc;
	bool	is_first;
	bool	is_last;
	char	*exec;
	char	**argv;
	t_list	*output_dest;
	t_list	*input_src;
}	t_parse_node;

typedef struct s_exec_node
{
	t_parse_node	*parse;
	int				fd_in;
	int				fd_out;
	int				**pipes;
	int				pindex;
	bool			run_cmd;
}	t_exec_node;

typedef struct s_data
{
	char	cwd[PATH_MAX];
	char	**path;
	char	**envp;
	int		exit;
}	t_data;

// TEST FUNCTIONS
void	run_execution_test(t_data *data);

// Parsing
void	parse(t_data *data, char *cmd);

// Execution
int		**create_pipes(int n);
void	execution(t_data *data, t_list *parse_nodes);
pid_t	*fork_processes(t_data *data, t_list *lst, int lsize);
int		waitpids(pid_t *pids, int n);
void	runcmd(t_data *data, t_exec_node *node);
void	runbuiltin(t_data *data, t_exec_node *node);

// Builtins
void	cd(t_data *data, char *cmd);

// Utils
void	cleanup(t_data *data, t_exec_node *enode, t_parse_node *pnode);
void	close_fds(int fd_in, int fd_out, int **pipes);
void	free_array(void **arr);
char	*ft_pathjoin(char const *s1, char const *s2);
void	print_prompt(t_data *data);

#endif
