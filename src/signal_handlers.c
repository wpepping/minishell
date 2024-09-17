/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 18:35:29 by wpepping          #+#    #+#             */
/*   Updated: 2024/09/17 17:51:05 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	default_sigint_handler(int signum)
{
	(void)signum;
	ft_putendl_fd("", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	process_running_sigint_handler(int signum)
{
	ft_putendl_fd("", STDOUT_FILENO);
	(void)signum;
}

void	sigquit_handler(int signum)
{
	ft_putendl_fd("Quit", STDOUT_FILENO);
	(void)signum;
}

void	init_signal_handlers(t_sigaction *sa_int, t_sigaction *sa_quit)
{
	(void)sa_quit;
	sigemptyset(&sa_int->sa_mask);
	sigaddset(&sa_int->sa_mask, SIGINT);
	sa_int->sa_flags = SA_RESTART;
	sa_int->sa_handler = default_sigint_handler;
	sigaction(SIGINT, sa_int, NULL);
	sigemptyset(&sa_quit->sa_mask);
	sigaddset(&sa_quit->sa_mask, SIGQUIT);
	sa_quit->sa_flags = SA_RESTART;
	sa_quit->sa_handler = SIG_IGN;
	sigaction(SIGQUIT, sa_quit, NULL);
}

void	switch_signal_handlers(t_sigact *sa_int, t_sigact *sa_quit, bool pr)
{
	if (pr)
	{
		sa_int->sa_handler = process_running_sigint_handler;
		sigaction(SIGINT, sa_int, NULL);
		sa_quit->sa_handler = sigquit_handler;
		sigaction(SIGQUIT, sa_quit, NULL);
	}
	else
	{
		sa_int->sa_handler = default_sigint_handler;
		sigaction(SIGINT, sa_int, NULL);
		sa_quit->sa_handler = SIG_IGN;
		sigaction(SIGQUIT, sa_quit, NULL);
	}
}
