#include "minishell.h"


int	get_builtin_index(char *token)
{
	int			i;
	const char	*builtin_str[] = {"echo", "cd", "unset", "export", "pwd", "env",
			"exit"};

	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(token, builtin_str[i], ft_strlen(builtin_str[i])) == 0)
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

bool	is_valid_filename(t_token *token)
{
	if (token->value == NULL || ft_strlen(token->value) == 0
		|| (token->type != WORD && token->type != DOUBLE_QUOTE
			&& token->type != SINGLE_QUOTE))
		return (false);
	return (true);
}

void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (token)
	{
		free(token->value);
		free(token);
	}
}

void	clear_tokens_list(t_list **tokens)
{
	if (tokens && *tokens)
	{
		ft_lstclear(tokens, free_token);
		*tokens = NULL;
	}
}

char	*generate_heredoc_filename(void)
{
	static int	i;
	char		*filename;
	char		*temp;

	temp = ft_itoa(i);
	malloc_protection(temp);
	filename = ft_strjoin("heredoc", temp);
	malloc_protection(filename);
	free(temp);
	i++;
	return (filename);
}
