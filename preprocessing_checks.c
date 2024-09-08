/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessing_checks.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 18:42:40 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/08 17:51:24 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	err_handl(char *msg, char *fname, t_data *data)
{
	char	*errmsg;
	t_list	*node;

	errmsg = ft_strdup(msg);
	if (fname != NULL)
		errmsg = ft_strjoin2(errmsg, fname);
	node = ft_lstnew(errmsg);
	if (node)
		ft_lstadd_back(&data->error_list, node);
	else
		ft_putendl_fd(errmsg, STDERR_FILENO);
	return (false);
}

bool	check_cmd(t_data *data, t_exec_node *node)
{
	char	**path;

	path = get_path();
	if (path == NULL)
		err_handl(ERR_OUT_OF_MEMORY, NULL, data);
	node->fullcmd = find_full_path(node->parse->argv[0], path);
	free_array((void **)path);
	if (node->parse->is_builtin)
		return (true);
	node->error_code = 127;
	if (node->fullcmd == NULL)
		return (err_handl(ERR_COMMAND_NOT_FOUND, node->parse->argv[0], data));
	if (access(node->fullcmd, F_OK) != 0)
		return (err_handl(ERR_NO_SUCH_FILE, node->parse->argv[0], data));
	node->error_code = 126;
	if (isdir(node->fullcmd))
		return (err_handl(ERR_IS_DIR, node->parse->argv[0], data));
	if (access(node->fullcmd, X_OK) != 0)
		return (err_handl(ERR_PERMISSION_DENIED, node->parse->argv[0], data));
	node->error_code = 0;
	return (true);
}

bool	check_fds(t_data *data, t_list *files, t_exec_node *enode)
{
	int		fd;
	t_token	*file;
	int		oflag;

	while (files)
	{
		file = (t_token *)files->content;
		oflag = oflags(file->type);
		fd = open(file->value, oflag, 0644);
		if ((fd == -1) && (file->type == REDIRECT_IN || file->type == HEREDOC)
			&& access(file->value, F_OK) != 0)
			return (err_handl(ERR_NO_SUCH_FILE, file->value, data));
		if (fd == -1)
			return (err_handl(ERR_PERMISSION_DENIED, file->value, data));
		if ((file->type == REDIRECT_IN || file->type == HEREDOC))
			enode->infile = file->value;
		else
			enode->outfile = file->value;
		close(fd);
		files = files->next;
	}
	return (true);
}
