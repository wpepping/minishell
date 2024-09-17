/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:03:29 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 13:45:55 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERR_COMMAND_NOT_FOUND "command not found"
# define ERR_OUT_OF_MEMORY "minishell: out of memory error"
# define ERR_NO_SUCH_FILE "no such file or directory"
# define ERR_PERMISSION_DENIED "permission denied"
# define ERR_IS_DIR "Is a directory"
# define ERR_CANNOT_EXEC "cannot execute binary file: Exec format error"
# define ERR_EXIT_TOO_MANY_ARG "minishell: exit: too many arguments"
# define PROMPT_END "$ "
# define DECLARE_X "declare -x "

typedef enum e_token_type
{
	WORD,
	HEREDOC,
	APPEND,
	REDIRECT_OUT,
	REDIRECT_IN,
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	PIPE
}							t_token_type;

typedef struct s_token
{
	char					*value;
	bool					inword;
	t_token_type			type;
}							t_token;

typedef struct s_execution
{
	t_list					*enodes;
	t_list					*pnodes;
	int						**pipes;
	int						lsize;
	int						nofork;
}							t_execution;

typedef struct s_parse_node
{
	bool					is_builtin;
	bool					is_last;
	bool					heredoc_fail;
	char					*exec;
	char					**argv;
	// t_list					*output_dest;
	// t_list					*input_src;
	t_list					*redirect;
}							t_parse_node;

typedef struct s_exec_node
{
	t_parse_node			*parse;
	t_list					*parse_nodes;
	int						fd_in;
	int						fd_out;
	int						**pipes;
	int						pindex;
	int						list_size;
	int						nofork;
	int						error_code;
	bool					run_cmd;
	char					*fullcmd;
	t_token					*infile;
	t_token					*outfile;
}							t_exec_node;

typedef struct s_data
{
	char					cwd[PATH_MAX];
	t_list					*node_list;
	int						exit;
	int						fd_stdin;
	int						fd_stdout;
	char					**envp;
	int						last_exit_code;
	t_list					*error_list;
}							t_data;

typedef struct sigaction	t_sigaction;
typedef t_sigaction			t_sigact;

// parse
int							parse(t_data *data, char *cmd);
// Tokenization
int							tokenize(char *cmd, t_list **token_list);

/// token_utils.c
int							append_token(t_list **token_list, t_token_type type,
								char *cmd, int len);
void						clear_tokens_list(t_list **tokens);
void						combine_inword(t_list **tokens);

// parse_handlers
t_list						*handle_redirects(t_list *tokens,
								t_parse_node *node, t_data data);
int							handle_heredoc(char *delimiter, t_data data,
								t_token *token);
void						handle_args(t_list *tokens, t_parse_node *node,
								int argc);
char						*handle_env(char *envpointer, t_data data,
								size_t len);

// expand_env
void						expand_env(t_token *token, char *envpointer,
								t_data data);

// parse utils
int							get_builtin_index(char *token);
bool						is_valid_filename(t_list *tokens);
int							get_tempfile_name(char *tempfile);

// heredoc
int							handle_heredoc(char *delimiter, t_data data,
								t_token *token);

void						print_prompt(t_data *data);
void						print_argv(t_parse_node *node);
void						print_argv_from_nodes(t_data *data);
void						clear_node_list(t_data *data);

// Pre-processing
bool						check_fds(t_data *data, t_list *files,
								t_exec_node *enode);
bool						check_cmd(t_data *data, t_exec_node *node);
t_list						*create_exec_nodes(t_data *data, t_execution *exec);

// Execution
int							**create_pipes(int n);
int							waitpids(pid_t *pids, int n);
int							oflags(t_token_type type);
void						execution(t_data *data, t_list *parse_nodes);
void						get_fds(t_data *data, t_exec_node *node,
								int **pipes);
pid_t						*fork_processes(t_data *data, t_execution *exec);
void						runcmd(t_data *data, t_exec_node *node);
int							runbuiltin(t_data *data, t_exec_node *node);

// Builtins
int							ft_echo(t_data *data, t_exec_node *node);
int							ft_cd(t_data *data, t_exec_node *node);
int							ft_pwd(t_data *data, t_exec_node *node);
int							ft_export(t_data *data, t_exec_node *node);
int							ft_unset(t_data *data, t_exec_node *node);
int							ft_env(t_data *data, t_exec_node *node);
int							ft_exit(t_data *data, t_exec_node *node);
void						invalid_option(char *command, char *option);

// Envp
char						**envp_create(char **envp);
int							envp_add(char ***envp, char *value, int n);
char						**envp_remove(char **envp, char **names);
int							envp_set(char ***envp, char *value);
char						*envp_get(char **envp, char *name);

// Signal handlers
void						default_sigint_handler(int signum);
void						process_running_sigint_handler(int signum);
void						sigquit_handler(int signum);
void						init_signal_handlers(t_sigaction *sa_int,
								t_sigaction *sa_quit);
// Utils
void						clean_exit(t_data *d, t_exec_node *enode,
								t_list *pnodes, int status);
void						close_fds(int fd_in, int fd_out, int **pipes);
void						ft_puterr(char *str1, char *str2, char *str3);
char						*ft_strjoin2(char *s1, char const *s2);
int							arrnstr(char **haystack, char *needle, int cmplen);
t_parse_node				*create_parse_node(void);
bool						isdir(char *dname);
int							ft_isint(char *str);
char						**get_path(t_data *data);
char						*find_full_path(char *cmd, char *path[]);
void						malloc_protection(void *ptr);
void						ft_safelst_add_back(void *content, t_list **list);
char						*ft_strchr2(char *str, char c);
int							ft_envncmp(const char *s1, const char *s2, size_t n);

// Clean up
void						cleanup(t_data *data);
void						cleanup_cmd(char *cmd, t_list *pnode);
void						cleanup_execution(t_execution *exec);
void						cleanup_run_one(void *node);
void						clean_heredocs(t_list *node);
void						free_array(void **arr);
void						free_parse_node(void *node);

#endif
