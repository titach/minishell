/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 13:27:56 by tchaloei          #+#    #+#             */
/*   Updated: 2024/12/24 13:30:42 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handle(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	signal (SIGQUIT, SIG_IGN);
	write (1, "\n", 1);
	rl_redisplay();
}

void	handle_sigint(int sig)
{
	(void)sig;
	signal (SIGQUIT, SIG_IGN);
	write(1, "\n", 1);
	// rl_on_new_line();
	// rl_replace_line("", 0);
	// signal (SIGQUIT, SIG_IGN);
	// write (1, "\n", 1);
	
	// char *cwd = getcwd(NULL, 0);
	// free(cwd)
}

void	handle_sigquit(int sig)
{
	(void)sig;
	write(2, "Quit (core dumped)\n", 19);
	// exit(0);
}

void	signal_init(int sig)
{
	if (sig == 1)
	{
		signal(SIGINT, sigint_handle);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (sig == 2)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, handle_sigquit);
	}
}
