#include "minishell.h"

bool	in_quotes(char *token)
{
	static bool	open_double_quote;
	static bool	open_single_quote;

	// char *first_quote;
	// char *last_quote;
	if (ft_strncmp(token, "\"", 1) == 0 && !open_single_quote)
		open_double_quote = !open_double_quote;
	else if (ft_strncmp(token, "'", 1) == 0 && !open_double_quote)
		open_single_quote = !open_single_quote;
	// else if(!open_single_quote)
	// {
	// 	first_quote = ft_strchr(token, '\"');
	// 	last_quote = ft_strrchr(token, '\"');
	// 	if(first_quote && first_quote == last_quote)
	// 		open_double_quote = !open_double_quote;
	// }
	// else if(!open_double_quote)
	// {
	// 	first_quote = ft_strchr(token, '\'');
	// 	last_quote = ft_strrchr(token, '\'');
	// 	if(first_quote && first_quote == last_quote)
	// 		open_single_quote = !open_single_quote;
	// }
	return (open_double_quote || open_single_quote);
}

bool	append_token(t_list **token_list, t_token_type type, char **cmd,
		int len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		printf("Error: malloc failed\n");
		return (false);
	}
	token->value = ft_substr(*cmd, 0, len);
	if (!token->value)
	{
		printf("Error: malloc failed\n");
		return (false);
	}
	token->type = type;
	printf("\ntoken value: %s\n", token->value);
	printf("\ntoken type: %i\n", token->type);
	ft_lstadd_back(token_list, ft_lstnew(token));
	while (len > 0)
	{
		(*cmd)++;
		len--;
	}
	return (true);
}

t_list	*tokenize(char *cmd)
{
	t_list	*token_list;
	int		i;

	// int		j;
	token_list = NULL;
	i = 0;
	while (*cmd)
	{
		if (*cmd == ' ')
			cmd++;
		else if (!(ft_strncmp(cmd, "&&", 2)) || !(ft_strncmp(cmd, "|", 1))
			|| !(ft_strncmp(cmd, ">", 1)) || !(ft_strncmp(cmd, "(", 1))
			|| !(ft_strncmp(cmd, ")", 1)) || !(ft_strncmp(cmd, "<", 1)))
		{
			if (ft_strncmp(cmd, "&&", 2) == 0)
				append_token(&token_list, AND, &cmd, 2);
			else if (ft_strncmp(cmd, "||", 2) == 0)
				append_token(&token_list, OR, &cmd, 2);
			else if (ft_strncmp(cmd, "<<", 2) == 0)
				append_token(&token_list, HEREDOC, &cmd, 2);
			else if (ft_strncmp(cmd, ">>", 2) == 0)
				append_token(&token_list, APPEND, &cmd, 2);
			else if (ft_strncmp(cmd, ">", 1) == 0)
				append_token(&token_list, REDIRECT_OUT, &cmd, 1);
			else if (ft_strncmp(cmd, "<", 1) == 0)
				append_token(&token_list, REDIRECT_IN, &cmd, 1);
			else if (ft_strncmp(cmd, "(", 1) == 0)
				append_token(&token_list, OPEN_PAREN, &cmd, 1);
			else if (ft_strncmp(cmd, ")", 1) == 0)
				append_token(&token_list, CLOSE_PAREN, &cmd, 1);
			else if (ft_strncmp(cmd, "|", 1) == 0)
				append_token(&token_list, PIPE, &cmd, 1);
		}
		else
		{
			while (cmd[i] && cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '<'
				&& cmd[i] != '>' && cmd[i] != '(' && cmd[i] != ')'
				&& cmd[i] != '&' && cmd[i] != '\t')
			{
				if (in_quotes(&cmd[i]))
				{
					i++;
					while (cmd[i] && in_quotes(&cmd[i]))
						i++;
					i++;
				}
				else
					i++;
			}
			append_token(&token_list, WORD, &cmd, i);
			i = 0;
		}
	}
	return (token_list);
}
