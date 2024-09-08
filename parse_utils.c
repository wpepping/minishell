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

bool	is_valid_filename(t_list *tokens)
{
	t_token	*token;

	if (!tokens || !tokens->content)
	{
		printf("Error: no filename specified for redirection\n");
		return (false);
	}
	token = (t_token *)tokens->content;
	if (token->value == NULL || ft_strlen(token->value) == 0
		|| (token->type != WORD && token->type != DOUBLE_QUOTE
			&& token->type != SINGLE_QUOTE))
	{
		printf("Error: invalid filename specified for redirection\n");
		return (false);
	}
	return (true);
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
