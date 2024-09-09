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

int get_tempfile_name(char *tempfile)
{
    int i;
    ft_memcpy(tempfile, "tmphered000", 12);
    i = 1;
    while (i < 1000)
    {
        tempfile[10] = '0' + (i % 10);
        tempfile[9] = '0' + ((i / 10) % 10);
        tempfile[8] = '0' + (i / 100);
        if (access(tempfile, F_OK) != 0)
            return (0);
        i++;
    }
    return (-1);
}

