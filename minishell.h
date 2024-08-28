/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:31 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/28 19:22:46 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <linux/limits.h>
# include <readline/readline.h>
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
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define ERR_COMMAND_NOT_FOUND "minishell: command not found: "
# define ERR_OUT_OF_MEMORY "minishell: out of memory error"
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
	PIPE
}					t_token_type;

typedef struct s_parse_node
{
	bool	is_builtin;
	bool	heredoc;
	bool	append;
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
	char			cwd[PATH_MAX];
	t_list			*node_list;
	int				exit;
	char			**envp;
}					t_data;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}					t_token;

// parse
void				parse(t_data *data, char *cmd);
void				parse_pipe(t_list **tokens, t_parse_node *node,
					t_data *data);
void				parse_args_and_redirects(t_list **tokens,
					t_parse_node *node);
void				parse_command(t_list *tokens, t_data *data);
t_parse_node		*create_parse_node(void);
void				print_prompt(t_data *data);
int					get_builtin_index(char *token);
int					get_args(t_list **tokens, t_parse_node *node);
void				handle_redirects(t_list **tokens, t_parse_node *node);
bool				in_quotes(char *token);
void				print_argv(t_parse_node *node);

void				print_argv_from_nodes(t_data *data);
void				clear_node_list(t_data *data);

// Tokenization
t_list				*tokenize(char *cmd);
bool				append_token(t_list **token_list, t_token_type type,
						char **cmd, int len);
bool				in_quotes(char *token);

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
int		ft_echo(t_data *data, t_exec_node *node);
int		ft_cd(t_data *data, t_exec_node *node);
int		ft_pwd(t_data *data, t_exec_node *node);
int		ft_export(t_data *data, t_exec_node *node);
int		ft_unset(t_data *data, t_exec_node *node);
int		ft_env(t_data *data, t_exec_node *node);
void	ft_exit(t_data *data, t_exec_node *node);
void	invalid_option(char *command, char *option);

// Envp
char	**envp_create(char **envp);
int		envp_add(char ***envp, char *value, int n);
char	**envp_remove(char **envp, char **names);
int		envp_set(char ***envp, char *value);
char	*envp_get(char **envp, char *name);

// Utils
void	clean_exit(char *msg, t_data *data,
			t_exec_node *enode, t_parse_node *pnode);
void	cleanup_for_exit(t_data *data, t_exec_node *enode, t_parse_node *pnode);
void	cleanup_cmd(t_data *data, t_exec_node *enode, t_parse_node *pnode);
void	close_fds(int fd_in, int fd_out, int **pipes);
void	free_array(void **arr);
char	*ft_pathjoin(char const *s1, char const *s2);
void	ft_putstrs_fd(char *str1, char *str2, char *str3, int fd);
char	*ft_strjoin2(char *s1, char const *s2);
int		arrncontains(char **haystack, char *needle, int cmplen);

#endif
