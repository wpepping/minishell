/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:03:29 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/09 15:10:04 by wpepping         ###   ########.fr       */
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

# define ERR_COMMAND_NOT_FOUND "minishell: command not found: "
# define ERR_OUT_OF_MEMORY "minishell: out of memory error"
# define ERR_NO_SUCH_FILE "minishell: no such file or directory: "
# define ERR_PERMISSION_DENIED "minishell: permission denied: "
# define ERR_IS_DIR "minishell: Is a directory: "
# define ERR_CANNOT_EXEC " cannot execute binary file: Exec format error"
# define ERR_EXIT_TOO_MANY_ARG "minishell: exit: too many arguments"
# define PROMPT_END "$ "

typedef enum e_token_type
{
	WORD,
	AND,
	OR,
	HEREDOC,
	APPEND,
	REDIRECT_OUT,
	REDIRECT_IN,
	OPEN_PAREN,
	CLOSE_PAREN,
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
	t_list	*enodes;
	t_list	*pnodes;
	int		**pipes;
	int		lsize;
	int		nofork;
}				t_execution;

typedef struct s_parse_node
{
	bool					is_builtin;
	bool					is_last;
	char					*exec;
	char					**argv;
	t_list					*output_dest;
	t_list					*input_src;
	t_list					*redirect;
}							t_parse_node;

typedef struct s_exec_node
{
	t_parse_node	*parse;
	t_list			*parse_nodes;
	int				fd_in;
	int				fd_out;
	int				**pipes;
	int				pindex;
	int				list_size;
	int				nofork;
	int				error_code;
	bool			run_cmd;
	char			*fullcmd;
	t_token			*infile;
	t_token			*outfile;
}				t_exec_node;

typedef struct s_data
{
	char		cwd[PATH_MAX];
	t_list		*node_list;
	int			exit;
	char		**envp;
	int			last_exit_code;
	t_list		*error_list;
}				t_data;

typedef struct sigaction	t_sigaction;

// parse
int							parse(t_data *data, char *cmd);
int							parse_pipe(t_list **tokens, t_data *data);
int							parse_command(t_list *tokens, t_data *data);
int							parse_redirects(t_list **tokens, t_parse_node *node);
void						parse_args(t_list **tokens, t_parse_node *node);

// parse_handlers
t_list						*handle_redirects(t_list *tokens,
								t_parse_node *node);
t_token						*handle_heredoc(char *delimiter);
void	handle_args(t_list *tokens, t_parse_node *node, int argc);
char						*handle_env(char *envpointer, t_data data,
								size_t len);


// expand_env
void						expand_env(t_token *token, char *envpointer,
								t_data data);
char						*add_until_env(char *start, char *expanded_str);
size_t						count_env_len(char *env_var);
size_t						count_to_next_env(char *start);
void						combine_inword(t_list **tokens);



// parse utils
int							get_builtin_index(char *token);
bool						is_valid_filename(t_token *token);
void						free_token(void *content);
void						clear_tokens_list(t_list **tokens);
char						*generate_heredoc_filename(void);

void						print_prompt(t_data *data);
void						print_argv(t_parse_node *node);
void						print_argv_from_nodes(t_data *data);
void						clear_node_list(t_data *data);

// Tokenization
int							tokenize(char *cmd, t_list **token_list);
int							append_token(t_list **token_list, t_token_type type,
								char *cmd, int len);
int							handle_other_tokens(char *cmd, t_list **token_list);
bool						in_quotes(char *token);
int							add_word(char *cmd, t_list **token_list);

// Pre-processing
bool			check_fds(t_data *data, t_list *files, t_exec_node *enode);
bool			check_cmd(t_data *data, t_exec_node *node);
t_list			*create_exec_nodes(t_data *data, t_execution *exec);

// Execution
int				**create_pipes(int n);
int				waitpids(pid_t *pids, int n);
int				oflags(t_token_type type);
void			execution(t_data *data, t_list *parse_nodes);
void			get_fds(t_data *data, t_exec_node *node, int **pipes);
pid_t			*fork_processes(t_data *data, t_execution *exec);
void			runcmd(t_data *data, t_exec_node *node);
int				runbuiltin(t_data *data, t_exec_node *node);

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
void			clean_exit(t_data *d, t_exec_node *enode, t_list *pnodes, int status);
void			close_fds(int fd_in, int fd_out, int **pipes);
char			*ft_pathjoin(char const *s1, char const *s2);
void			ft_puterr(char *str1, char *str2, char *str3);
char			*ft_strjoin2(char *s1, char const *s2);
int				arrncontains(char **haystack, char *needle, int cmplen);
t_parse_node	*create_parse_node(void);
bool			isdir(char *dname);
int				ft_isint(char *str);
char			**get_path(void);
char			*find_full_path(char *cmd, char *path[]);
void			malloc_protection(void *ptr);

// Clean up
void						cleanup(t_data *data, t_exec_node *enode,
								t_parse_node *pnode);
void						cleanup_cmd(t_list *pnode);
void						free_array(void **arr);

#endif
