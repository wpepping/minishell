/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:27:40 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 17:27:42 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include <errno.h>
# include <linux/limits.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include <signal.h>
# include <bits/sigaction.h>

# define ERR_COMMAND_NOT_FOUND "minishell: command not found: "
# define ERR_OUT_OF_MEMORY "minishell: out of memory error"

# define PROMPT_END "$ "

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
	int				nofork;
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
void	cd(t_data *data, char *cmd);
void	echo(char *cmd);

// Parsing
void	parse(t_data *data, char *cmd);

// Execution
int		**create_pipes(int n);
void	execution(t_data *data, t_list *parse_nodes);
void	get_fds(t_data *data, t_exec_node *node, int **pipes);
pid_t	*fork_processes(t_data *data, t_list *lst, int lsize);
int		waitpids(pid_t *pids, int n);
void	runcmd(t_data *data, t_exec_node *node);
void	runbuiltin(t_data *data, t_exec_node *node);

// Builtins
void	ft_echo(t_data *data, t_exec_node *node);
int		ft_cd(t_data *data, t_exec_node *node);
void	ft_pwd(t_data *data, t_exec_node *node);
void	ft_export(t_data *data, t_exec_node *node);
void	ft_unset(t_data *data, t_exec_node *node);
void	ft_env(t_data *data, t_exec_node *node);
void	invalid_option(char *command, char *option);

// Envp
char	**envp_create(char **envp);
void	envp_add(char ***envp, char *value, int n);
char	**envp_remove(char **envp, char **names);
void	envp_set(char ***envp, char *value);
char	*envp_get(char **envp, char *name);

// Utils
void	cleanup_exit(t_data *data, t_exec_node *enode, t_parse_node *pnode);
void	cleanup_cmd(t_data *data, t_exec_node *enode, t_parse_node *pnode);
void	close_fds(int fd_in, int fd_out, int **pipes);
void	free_array(void **arr);
char	*ft_pathjoin(char const *s1, char const *s2);
void	ft_putstrs_fd(char *str1, char *str2, char *str3, int fd);
char	*ft_strjoin2(char *s1, char const *s2);
int		arrncontains(char **haystack, char *needle, int cmplen);

#endif
