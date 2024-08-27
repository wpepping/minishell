/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:24 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 17:31:40 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data)
{
	data->node_list = NULL;
	data->exit = 0;
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
			printf("Output: %s\n", (char *)output_dest->content);
			output_dest = output_dest->next;
		}
		input_src = node->input_src;
		while (input_src)
		{
			printf("Input: %s\n", (char *)input_src->content);
			input_src = input_src->next;
		}
		if (node->is_last)
			printf("Last node\n");
		current = current->next;
	}
}

int	main(void)
{
	t_data				data;
	char				*cmd;
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = signal_handler;
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	init(&data);
	while (!data.exit)
	{
		print_prompt(&data);
		cmd = readline(NULL);
		parse(&data, cmd);
		free(cmd);
		print_argv_from_nodes(&data);
		// clear_node_list(&data);
	}
	return (0);
}
