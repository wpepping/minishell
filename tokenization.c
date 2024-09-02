#include "minishell.h"

int	append_token(t_list **token_list, t_token_type type, char *cmd, int len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	malloc_protection(token);
	token->value = ft_substr(cmd, 0, len);
	malloc_protection(token->value);
	token->type = type;
	ft_lstadd_back(token_list, ft_lstnew(token));
	return (len);
}

void	malloc_protection(void *ptr)
{
	if (!ptr)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
}

int	add_quote(char *cmd, char quote, t_list **token_list)
{
	int	i;

	i = 1;
	if (strchr(cmd + 1, quote) == NULL)
	{
		printf("Error: no matching quote\n");
		return (0);
	}
	while (cmd[i] && cmd[i] != quote)
		i++;
	if (quote == '"')
		append_token(token_list, DOUBLE_QUOTE, cmd, i + 1);
	else if (quote == '\'')
		append_token(token_list, SINGLE_QUOTE, cmd, i + 1);
	return (i + 1);
}

int	add_word(char *cmd, t_list **token_list)
{
	int	i;

	i = 0;
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '<'
		&& cmd[i] != '>' && cmd[i] != '(' && cmd[i] != ')' && cmd[i] != '&'
		&& cmd[i] != '\t' && cmd[i] != '"' && cmd[i] != '\'')
	{
		i++;
	}
	append_token(token_list, WORD, cmd, i);
	return (i);
}

int	handle_other_tokens(char *cmd, t_list **token_list)
{
	if (ft_strncmp(cmd, "&&", 2) == 0)
		return (append_token(token_list, AND, cmd, 2));
	else if (ft_strncmp(cmd, "||", 2) == 0)
		return (append_token(token_list, OR, cmd, 2));
	else if (ft_strncmp(cmd, "(", 1) == 0)
		return (append_token(token_list, OPEN_PAREN, cmd, 1));
	else if (ft_strncmp(cmd, ")", 1) == 0)
		return (append_token(token_list, CLOSE_PAREN, cmd, 1));
	else if (ft_strncmp(cmd, "<", 1) == 0)
		return (append_token(token_list, REDIRECT_IN, cmd, 1));
	else if (ft_strncmp(cmd, ">", 1) == 0)
		return (append_token(token_list, REDIRECT_OUT, cmd, 1));
	return (0);
}

t_list	*tokenize(char *cmd)
{
	t_list	*token_list;

	token_list = NULL;
	while (*cmd)
	{
		if (*cmd == ' ' || *cmd == '\t')
			cmd++;
		else if (ft_strncmp(cmd, "\"", 1) == 0)
			cmd += add_quote(cmd, '"', &token_list);
		else if (ft_strncmp(cmd, "'", 1) == 0)
			cmd += add_quote(cmd, '\'', &token_list);
		else if (ft_strncmp(cmd, "<<", 2) == 0)
			cmd += append_token(&token_list, HEREDOC, cmd, 2);
		else if (ft_strncmp(cmd, ">>", 2) == 0)
			cmd += append_token(&token_list, APPEND, cmd, 2);
		else if (ft_strncmp(cmd, "&&", 2) == 0 || ft_strncmp(cmd, "||", 2) == 0
			|| ft_strncmp(cmd, "(", 1) == 0 || ft_strncmp(cmd, ")", 1) == 0
			|| ft_strncmp(cmd, "<", 1) == 0 || ft_strncmp(cmd, ">", 1) == 0)
			cmd += handle_other_tokens(cmd, &token_list);
		else if (ft_strncmp(cmd, "|", 1) == 0)
			cmd += append_token(&token_list, PIPE, cmd, 1);
		else
			cmd += add_word(cmd, &token_list);
	}
	return (token_list);
}
