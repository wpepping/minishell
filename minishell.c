/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:24 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/04 14:02:14 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data, char **envp)
{
	data->node_list = NULL;
	data->exit = 0;
	data->envp = envp_create(envp);
	if (!data->envp)
		clean_exit(ERR_OUT_OF_MEMORY, data, NULL, NULL);
	getcwd(data->cwd, PATH_MAX);
}

void	signal_handler(int signum)
{
	(void)signum;
	printf("\ninterrupted\n");
}

void	print_argv_from_nodes(t_data *data)
{
	t_list			*current;
	int				j;
	t_parse_node	*node;
	char			**argv;
	t_list			*output_dest;
	t_list			*input_src;

	current = data->node_list;
	j = 0;
	while (current != NULL)
	{
		node = (t_parse_node *)current->content;
		argv = node->argv;
		printf("Node %d\n", j++);
		printf("Arguments:\n");
		for (int i = 0; argv[i] != NULL; i++)
		{
			printf("  %s\n", argv[i]);
		}
		// Use temporary pointers to iterate through the lists
		output_dest = node->output_dest;
		while (output_dest)
		{
			printf("Output: %s type: %i\n", ((t_token *)output_dest->content)->value, ((t_token *)output_dest->content)->type);
			
			output_dest = output_dest->next;
		}
		input_src = node->input_src;
		while (input_src)
		{
			printf("Input: %s\n", ((t_token *)input_src->content)->value);
			input_src = input_src->next;
		}
		if (node->is_last)
			printf("Last node\n");
		current = current->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data				data;
	char				*cmd;
	char				*prompt;
	struct sigaction	sa;

	(void)argc;
	(void)argv;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = signal_handler;
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	init(&data, envp);
	while (!data.exit)
	{
		prompt = ft_strjoin(data.cwd, PROMPT_END);
		cmd = readline(prompt);
		free(prompt);
		if (*cmd != '\0')
		{
			add_history(cmd);
			parse(&data, cmd);
			//print_argv_from_nodes(&data);
			execution(&data, data.node_list);
			free(cmd);
			data.node_list = NULL;
		}
		//clear list
		
	}
	return (0);
}
