/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:11:46 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/08 17:24:19 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_data *data, char **envp)
{
	data->node_list = NULL;
	data->exit = 0;
	data->envp = envp_create(envp);
	data->last_exit_code = 0;
	data->error_list = NULL;
	if (!data->envp)
	{
		ft_putendl_fd(ERR_OUT_OF_MEMORY, STDERR_FILENO);
		clean_exit(data, NULL, NULL);
	}
	getcwd(data->cwd, PATH_MAX);
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
			printf("Input TYPE: %i\n", ((t_token *)input_src->content)->type);
			input_src = input_src->next;
		}
		if (node->is_last)
			printf("Last node\n");
		current = current->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data		data;
	char		*cmd;
	char		*prompt;
	t_sigaction	sa_int;
	t_sigaction	sa_quit;

	(void)argc;
	(void)argv;
	init_signal_handlers(&sa_int, &sa_quit);
	init(&data, envp);
	while (!data.exit)
	{
		prompt = ft_strjoin(data.cwd, PROMPT_END);
		cmd = readline(prompt);
		free(prompt);
		if (!cmd)
			data.exit = 1;
		else if (*cmd != '\0')
		{
			add_history(cmd);
			//print_argv_from_nodes(&data);
			sa_int.sa_handler = process_running_sigint_handler;
			sigaction(SIGINT, &sa_int, NULL);
			if(!parse(&data, cmd))
				execution(&data, data.node_list);
			sa_int.sa_handler = default_sigint_handler;
			sigaction(SIGINT, &sa_int, NULL);
			free(cmd);
			cleanup_cmd(data.node_list);
			data.node_list = NULL;
		}
	}
	cleanup(&data, NULL, NULL);
	return (data.last_exit_code);
}
