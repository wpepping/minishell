#include minishell.h

char	*handle_env(char *envpointer, t_data data, size_t len)
{
	char	*temp;
	char	*env_var;
	char	*env_val;

	if (len == 0)
		env_var = strdup("$");
	else
	{
		temp = ft_substr(envpointer, 0, len);
		malloc_protection(temp);
		if (temp[0] == '?')
		{
			env_var = ft_itoa(data.last_exit_code);
			malloc_protection(env_var);
		}
		else
		{
			env_val = envp_get(data.envp, temp);
			if (env_val)
			{
				env_var = ft_strdup(env_val);
				malloc_protection(env_var);
			}
			else
				env_var = NULL;
		}
		free(temp);
	}
	return (env_var);
}

char	*add_until_env(char *start, char *expanded_str)
{
	size_t	len;
	char	*temp;

	len = count_to_next_env(start);
	if (len)
	{
		temp = ft_substr(start, 0, len);
		malloc_protection(temp);
		expanded_str = ft_strjoin2(expanded_str, temp);
		malloc_protection(expanded_str);
		free(temp);
	}
	return (expanded_str);
}

size_t	count_env_len(char *env_var)
{
	size_t	i;

	i = 0;
	while (env_var[i] && (ft_isalnum(env_var[i]) || env_var[i] == '_'
			|| env_var[0] == '?' || env_var[1] == '$')
		&& !ft_isdigit(env_var[0]))
		i++;
	return (i);
}

void	expand_env(t_token *token, char *envpointer, t_data data)
{
	char	*expanded_str;
	char	*env_var;
	size_t	len;

	// char	*temp;
	expanded_str = ft_strdup("");
	expanded_str = add_until_env(token->value, expanded_str);
	while (envpointer)
	{
		len = count_env_len(envpointer + 1);
		env_var = handle_env(envpointer + 1, data, len);
		if (env_var)
		{
			expanded_str = ft_strjoin2(expanded_str, env_var);
			malloc_protection(expanded_str);
			free(env_var);
		}
		if (token->type == DOUBLE_QUOTE)
			expanded_str = add_until_env(envpointer + len + 1, expanded_str);
		envpointer = ft_strchr(envpointer + 1, '$');
	}
	token->value = ft_strdup(expanded_str);
}

size_t	count_to_next_env(char *start)
{
	size_t	i;

	i = 0;
	while (start[i] && start[i] != '$')
		i++;
	return (i);
}

