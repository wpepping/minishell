/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:22:33 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/20 19:09:01 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse(t_data *data, char *cmd)
{
	int				strlen;
	t_parse_node	*node;
	char			**tokens;
	int				i;

	i = 0;
	if (cmd == NULL || ft_strncmp(cmd, "exit", 5) == 0)
	{
		data->exit = 1;
		return ;
	}
	tokens = ft_split(cmd, ' ');
	node = create_parse_node();
	while (tokens[i])
	{
		if (access(tokens[i], X_OK) == 0)
		{
			node->exec = ft_strdup(tokens[i]);
			i += get_args(i, tokens, node);
		}
		else if (get_builtin_index(tokens[i]) != -1)
		{
			node->is_builtin = true;
			node->exec = ft_strdup(tokens[i]);
			i += get_args(i, tokens, node);
		}
		if(tokens[i] && strcmp(tokens[i], "<") == 0)
		{
			i++;
			node->input_src = ft_split(tokens[i], ' ');
			i++;
		}
		i++;
	}
	strlen = ft_strlen(cmd);
	if (strlen > 0)
		printf("minishell: command not found: %s\n", cmd);
	else
		printf("\n");
}

t_parse_node	*create_parse_node(void)
{
	t_parse_node	*node;

	node = malloc(sizeof(t_parse_node));
	if (!node)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	node->is_builtin = false;
	node-> append = false;
	node->exec = NULL;
	node->argv = NULL;
	node->output_dest = STDOUT_FILENO;
	node->input_src = STDIN_FILENO;
	return (node);
}

int	get_builtin_index(char *token)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(token, builtin_str[i], ft_strlen(builtin_str[i])) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	get_args(int index, char **tokens, t_parse_node *node)
{
	int	i;
	int argc;
	argc = 0;
	i = 0;

	while (tokens[index + argc] && strcmp(tokens[index + argc], "<") != 0 && strcmp(tokens[index + argc], ">") != 0 && strcmp(tokens[index], "|") != 0)
        argc++;
	node->argv = malloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		node->argv[i] = strdup(tokens[index]);
		if(!node->argv[i])
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		i++;
		index++;
	}
	node->argv[i] = NULL;
	return (argc);
}

void handle_redirects(char **tokens, int index, t_parse_node *node)
{
	if (ft_strncmp(tokens[index], ">>", 2) == 0)
		node->append = true;
	if (ft_strncmp(tokens[index], ">", 1) == 0)
		node->input_src = ft_strdup(tokens[index + 1]);
	if (ft_strncmp(tokens[index], "<<", 2) == 0)
		node->heredoc = true;
	if (ft_strncmp(tokens[index], "<", 1) == 0)
		node->input_src = ft_strdup(tokens[index + 1]);
}

	

