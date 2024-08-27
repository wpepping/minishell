/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:06:24 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 02:09:19 by phartman         ###   ########.fr       */
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


// void clear_node_list(t_data *data) {
//     t_list *current = data->node_list;
//     t_list *next;

//     while (current != NULL) {
//         t_parse_node *node = (t_parse_node *)current->content;
//         char **argv = node->argv;

//         // Free each string in the argv array
//         for (int i = 0; argv[i] != NULL; i++) {
//             free(argv[i]);
//         }
//         // Free the argv array itself
//         free(argv);

//         // Free the node structure
//         free(node);

//         // Move to the next node
//         next = current->next;
//         free(current);
//         current = next;
//     }

//     // Set the node_list to NULL
//     data->node_list = NULL;
// }


void print_argv_from_nodes(t_data *data) {
    t_list *current = data->node_list;
	int j = 0;
    while (current != NULL) {
        t_parse_node *node = (t_parse_node *)current->content;
        char **argv = node->argv;
		printf("Node %d\n", j++);
        printf("Arguments:\n");
        for (int i = 0; argv[i] != NULL; i++) {
            printf("  %s\n", argv[i]);
        }
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
		//clear_node_list(&data);
		
	}
	return (0);
}
