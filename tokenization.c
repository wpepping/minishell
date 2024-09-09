#include "minishell.h"

static int	handle_quotes(char **cmd, t_list **token_list);
static int	handle_other_tokens(char *cmd, t_list **token_list);
static int	add_word(char *cmd, t_list **token_list);
static int	add_quote(char *cmd, char quote, t_list **token_list);

int	tokenize(char *cmd, t_list **token_list)
{
	*token_list = NULL;
	while (*cmd)
	{
		if (*cmd == ' ' || *cmd == '\t')
		{
			cmd++;
			((t_token *)ft_lstlast(*token_list)->content)->inword = false;
		}
		else if (ft_strncmp(cmd, "\"", 1) == 0 || ft_strncmp(cmd, "'", 1) == 0
				|| ft_strncmp(cmd, "||", 2) == 0)
		{
			if (handle_quotes(&cmd, token_list) == -1)
				return (1);
		}
		else if (ft_strncmp(cmd, ">", 1) == 0 || ft_strncmp(cmd, "<", 1) == 0
			|| ft_strncmp(cmd, "|", 1) == 0)
			cmd += handle_other_tokens(cmd, token_list);
		else
			cmd += add_word(cmd, token_list);
	}
	return (0);
}

static int	add_quote(char *cmd, char quote, t_list **token_list)
{
	int	i;

	i = 1;
	if (strchr(cmd + 1, quote) == NULL)
	{
		printf("Error: no matching quote\n");
		return (-1);
	}
	while (cmd[i] && cmd[i] != quote)
		i++;
	if (quote == '"')
		append_token(token_list, DOUBLE_QUOTE, cmd, i + 1);
	else if (quote == '\'')
		append_token(token_list, SINGLE_QUOTE, cmd, i + 1);
	return (i + 1);
}

static int	add_word(char *cmd, t_list **token_list)
{
	int	i;

	i = 1;
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '<'
		&& cmd[i] != '>' && cmd[i] != '(' && cmd[i] != ')' && cmd[i] != '\t'
		&& cmd[i] != '"' && cmd[i] != '\'' && cmd[i] != '\n')
	{
		i++;
	}
	append_token(token_list, WORD, cmd, i);
	((t_token *)ft_lstlast(*token_list)->content)->inword = true;
	return (i);
}

static int	handle_other_tokens(char *cmd, t_list **token_list)
{
	if(*token_list)
		((t_token *)ft_lstlast(*token_list)->content)->inword = false;
	if (ft_strncmp(cmd, "<<", 2) == 0)
		return (append_token(token_list, HEREDOC, cmd, 2));
	else if (ft_strncmp(cmd, ">>", 2) == 0)
		return (append_token(token_list, APPEND, cmd, 2));
	else if (ft_strncmp(cmd, "<", 1) == 0)
		return (append_token(token_list, REDIRECT_IN, cmd, 1));
	else if (ft_strncmp(cmd, ">", 1) == 0)
		return (append_token(token_list, REDIRECT_OUT, cmd, 1));
	else if (ft_strncmp(cmd, "|", 1) == 0)
		return (append_token(token_list, PIPE, cmd, 1));
	return (0);
}

static int	handle_quotes(char **cmd, t_list **token_list)
{
	int	result;

	if (ft_strncmp(*cmd, "\"", 1) == 0)
	{
		result = add_quote(*cmd, '"', token_list);
		if (result == -1)
			return (-1);
		else
			*cmd += result;
	}
	else if (ft_strncmp(*cmd, "'", 1) == 0)
	{
		result = add_quote(*cmd, '\'', token_list);
		if (result == -1)
			return (-1);
		else
			*cmd += result;
	}
	else if (ft_strncmp(*cmd, "||", 2) == 0)
	{
		printf("Error: syntax error near unexpected token '|'\n");
		return (-1);
	}
	((t_token *)ft_lstlast(*token_list)->content)->inword = true;
	return (0);
}
