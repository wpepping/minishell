/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:31 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 14:48:07 by phartman         ###   ########.fr       */
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
	t_list	*node_list;
	int		exit;
}	t_data;

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
}	t_token_type;

typedef struct s_token
{
	char	*value;
	t_token_type type;
}	t_token;

typedef struct s_parse_node
{
	bool	is_builtin;
	bool is_last;
	bool 	append;
	bool 	heredoc;
	char	*exec;
	char 	**argv;
	t_list 	*output_dest;
	t_list 	*input_src;
	
}	t_parse_node;



//parse
void	parse(t_data *data, char *cmd);
void parse_pipe(t_list **tokens, t_parse_node *node , t_data *data);
void	parse_args_and_redirects(t_list **tokens, t_parse_node *node);
void	parse_command(t_list *tokens, t_data *data);
t_parse_node	*create_parse_node(void);
void	print_prompt(t_data *data);
int	get_builtin_index(char *token);
int	get_args(t_list **tokens, t_parse_node *node);
void	handle_redirects(t_list **tokens, t_parse_node *node);
void print_argv(t_parse_node *node);

void print_argv_from_nodes(t_data *data);
void clear_node_list(t_data *data);

//Tokenization
t_list	*tokenize(char *cmd);
bool	append_token(t_list **token_list, t_token_type type, char **cmd,
		int len);
bool	in_quotes(char *token);


// Builtins
void	cd(t_data *data, char *cmd);
void	echo(char *cmd);


#endif
