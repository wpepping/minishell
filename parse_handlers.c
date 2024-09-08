#include "minishell.h"

static int	process_line(char *line, char *delimiter, t_data data, int fd)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!line || (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\0'))
	{
		free(line);
		return (0);
	}
	token->value = line;
	if (ft_strchr(line, '$'))
	{
		expand_env(token, ft_strchr(line, '$'), data);
		free(line);
	}
	write(fd, token->value, ft_strlen(token->value));
	write(fd, "\n", 1);
	free(token->value);
	free(token);
	return (1);
}

t_token	*handle_heredoc(char *delimiter, t_data data)
{
	int		fd;
	char	*filename;
	t_token	*token;
	int		continue_reading;

	token = malloc(sizeof(t_token));
	malloc_protection(token);
	filename = generate_heredoc_filename();
	token->value = filename;
	token->type = HEREDOC;
	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("Error: could not create heredoc file\n");
		free(filename);
		exit(1);
	}
	continue_reading = 1;
	while (continue_reading)
		continue_reading = process_line(readline("> "), delimiter, data, fd);
	close(fd);
	free(delimiter);
	return (token);
}

t_list	*handle_redirects(t_list *tokens, t_parse_node *node, t_data data)
{
	t_token	*token;
	t_list	*current;
	t_token	*content_copy;

	current = tokens;
	token = (t_token *)current->content;
	content_copy = malloc(sizeof(t_token));
	malloc_protection(content_copy);
	content_copy->type = token->type;
	content_copy->value = ft_strdup(((t_token *)(current->next->content))->value);
	malloc_protection(content_copy->value);
	if (token->type == REDIRECT_IN || token->type == HEREDOC)
	{
		if (token->type == HEREDOC)
			content_copy = handle_heredoc(content_copy->value, data);
		ft_lstadd_back(&node->input_src, ft_lstnew(content_copy));
	}
	else if (token->type == REDIRECT_OUT || token->type == APPEND)
	{
		ft_lstadd_back(&node->output_dest, ft_lstnew(content_copy));
	}
	if (current->next != NULL)
		return (current->next->next);
	else
		return (current->next);
}

void	handle_args(t_list *tokens, t_parse_node *node, int argc)
{
	int		i;
	t_token	*token;

	token = (t_token *)tokens->content;
	i = 0;
	node->argv = malloc(sizeof(char *) * (argc + 1));
	while (i < argc && tokens)
	{
		token = (t_token *)tokens->content;
		if (token->type == REDIRECT_IN || token->type == REDIRECT_OUT
			|| token->type == APPEND || token->type == HEREDOC)
		{
			tokens = tokens->next->next;
			continue ;
		}
		if (token->type == WORD)
		{
			node->argv[i] = strdup(token->value);
			malloc_protection(node->argv[i]);
			i++;
		}
		tokens = tokens->next;
	}
	node->argv[i] = NULL;
}

char	*handle_env(char *envpointer, t_data data, size_t len)
{
	char	*temp;
	char	*env_var;
	char	*env_val;

	if (len == 0)
		return (ft_strdup("$"));
	temp = ft_substr(envpointer, 0, len);
	malloc_protection(temp);
	if (temp[0] == '?')
		env_var = ft_itoa(data.last_exit_code);
	else
	{
		env_val = envp_get(data.envp, temp);
		if (!env_val)
		{
			free(temp);
			return (NULL);
		}
		else
			env_var = ft_strdup(env_val);
	}
	malloc_protection(env_var);
	free(temp);
	return (env_var);
}
