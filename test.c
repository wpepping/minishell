/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:13:41 by wpepping          #+#    #+#             */
/*   Updated: 2024/08/27 17:49:30 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_execution_test_0(t_data *data)
{
	t_parse_node	p0;
	t_list			*lst;

	lst = NULL;

	p0.argv = malloc(3 * sizeof(char *));
	p0.argv[0] = ft_strdup("env");
	p0.argv[1] = NULL;
	p0.is_builtin = 1;
	p0.heredoc = 0;
	p0.is_last = 1;
	p0.exec = NULL;
	p0.input_src = NULL;
	p0.output_dest = NULL;
	ft_lstadd_front(&lst, ft_lstnew(&p0));

	execution(data, lst);
}

void	run_execution_test(t_data *data)
{
	t_parse_node	p0;
	t_parse_node	p1;
	t_parse_node	p2;
	t_list			*lst;

	lst = NULL;

	p0.argv = malloc(3 * sizeof(char *));
	p0.argv[0] = ft_strdup("cat");
	p0.argv[1] = NULL;
	p0.is_builtin = 0;
	p0.heredoc = 0;
	p0.is_last = 0;
	p0.exec = NULL;
	p0.input_src = NULL;
	p0.output_dest = NULL;

	p1.argv = malloc(3 * sizeof(char *));
	p1.argv[0] = ft_strdup("cat");
	p1.argv[1] = NULL;
	p1.is_builtin = 0;
	p1.heredoc = 0;
	p1.is_last = 0;
	p1.exec = NULL;
	p1.input_src = NULL;
	p1.output_dest = NULL;

	p2.argv = malloc(2 * sizeof(char *));
	p2.argv[0] = ft_strdup("ls");
	p2.argv[1] = NULL;
	p2.is_builtin = 0;
	p2.heredoc = 0;
	p2.is_last = 1;
	p2.exec = NULL;
	p2.input_src = NULL;
	p2.output_dest = NULL;

	ft_lstadd_front(&lst, ft_lstnew(&p2));
	ft_lstadd_front(&lst, ft_lstnew(&p1));
	ft_lstadd_front(&lst, ft_lstnew(&p0));

	execution(data, lst);
}

void	run_execution_test2(t_data *data)
{
	t_parse_node	p0;
	t_parse_node	p1;
	t_parse_node	p2;
	t_list			*lst;

	lst = NULL;

	p0.argv = malloc(3 * sizeof(char *));
	p0.argv[0] = ft_strdup("rm");
	p0.argv[1] = ft_strdup("a");
	p0.argv[2] = NULL;
	p0.is_builtin = 0;
	p0.heredoc = 0;
	p0.is_last = 0;
	p0.exec = NULL;
	p0.input_src = NULL;
	p0.output_dest = NULL;

	p1.argv = malloc(3 * sizeof(char *));
	p1.argv[0] = ft_strdup("rm");
	p1.argv[1] = ft_strdup("b");
	p1.argv[2] = NULL;
	p1.is_builtin = 0;
	p1.heredoc = 0;
	p1.is_last = 0;
	p1.exec = NULL;
	p1.input_src = NULL;
	p1.output_dest = NULL;

	p2.argv = malloc(2 * sizeof(char *));
	p2.argv[0] = ft_strdup("rm");
	p2.argv[1] = ft_strdup("c");
	p2.argv[2] = NULL;
	p2.is_builtin = 0;
	p2.heredoc = 0;
	p2.is_last = 1;
	p2.exec = NULL;
	p2.input_src = NULL;
	p2.output_dest = NULL;

	ft_lstadd_front(&lst, ft_lstnew(&p2));
	ft_lstadd_front(&lst, ft_lstnew(&p1));
	ft_lstadd_front(&lst, ft_lstnew(&p0));
	ft_lstadd_front(&p1.input_src, ft_lstnew("input.txt"));
	ft_lstadd_front(&p0.input_src, ft_lstnew("input5.txt"));
	ft_lstadd_front(&p1.input_src, ft_lstnew("input1.txt"));
	ft_lstadd_front(&p1.input_src, ft_lstnew("input.txt"));
	execution(data, lst);
}

void	run_execution_test3(t_data *data)
{
	t_parse_node	p0;
	t_parse_node	p1;
	t_parse_node	p2;
	t_list			*lst;

	lst = NULL;

	p0.argv = malloc(3 * sizeof(char *));
	p0.argv[0] = ft_strdup("ls");
	p0.argv[1] = ft_strdup("-l");
	p0.argv[2] = NULL;
	p0.is_builtin = 0;
	p0.heredoc = 0;
	p0.is_last = 0;
	p0.exec = NULL;
	p0.input_src = NULL;
	p0.output_dest = NULL;

	p1.argv = malloc(3 * sizeof(char *));
	p1.argv[0] = ft_strdup("cat");
	p1.argv[1] = NULL;
	p1.is_builtin = 0;
	p1.heredoc = 0;
	p1.is_last = 0;
	p1.exec = NULL;
	p1.input_src = NULL;
	p1.output_dest = NULL;

	p2.argv = malloc(2 * sizeof(char *));
	p2.argv[0] = ft_strdup("ls");
	p2.argv[1] = ft_strdup("-l");
	p2.argv[2] = NULL;
	p2.is_builtin = 0;
	p2.heredoc = 0;
	p2.is_last = 1;
	p2.exec = NULL;
	p2.input_src = NULL;
	p2.output_dest = NULL;

	ft_lstadd_front(&lst, ft_lstnew(&p2));
	ft_lstadd_front(&lst, ft_lstnew(&p1));
	ft_lstadd_front(&lst, ft_lstnew(&p0));
	ft_lstadd_front(&p0.output_dest, ft_lstnew("output.txt"));
	ft_lstadd_front(&p1.input_src, ft_lstnew("input1.txt"));
	ft_lstadd_front(&p1.input_src, ft_lstnew("input.txt"));
	ft_lstadd_front(&p1.output_dest, ft_lstnew("output1.txt"));
	execution(data, lst);
}

void	cd(t_data *data, char *cmd)
{
	char	*arg;
	char	*base;
	char	*path;

	arg = ft_substr(cmd, 3, ft_strlen(cmd));
	if (arg[0] == '/')
		base = ft_strdup("");
	else
		base = ft_strjoin(data->cwd, "/");
	path = ft_strjoin(base, arg);
	chdir(path);
	free(base);
	free(path);
	getcwd(data->cwd, PATH_MAX);
}
