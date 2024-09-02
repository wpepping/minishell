#include "minishell.h"


t_token	*handle_heredoc(char *delimiter)
{
	char	*line;
	int		fd;
	char	*filename;
	t_token	*token;

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
	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	free(delimiter);
	return (token);
}

t_list	*handle_redirects(t_list *tokens, t_parse_node *node)
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
			content_copy = handle_heredoc(content_copy->value);
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

int	get_args(t_list **tokens, t_parse_node *node)
{
	int		i;
	int		argc;
	t_list	*tmp;
	t_token	*token;

	token = (t_token *)(*tokens)->content;
	tmp = *tokens;
	argc = 0;
	i = 0;
	token = (t_token *)tmp->content;
	while (tmp && (token->type == WORD || token->type == DOUBLE_QUOTE
			|| token->type == SINGLE_QUOTE))
	{
		argc++;
		if (tmp->next == NULL)
			break ;
		tmp = tmp->next;
		token = (t_token *)tmp->content;
	}
	token = (t_token *)(*tokens)->content;
	node->argv = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		node->argv[i] = strdup(token->value);
		malloc_protection(node->argv[i]);
		i++;
		*tokens = (*tokens)->next;
		if (*tokens)
			token = (t_token *)(*tokens)->content;
	}
	node->argv[i] = NULL;
	return (argc);
}
