/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 13:27:56 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/27 15:29:03 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handle(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	signal (SIGQUIT, SIG_IGN);
	write (1, "\n", 1);
	rl_redisplay();
	g_ss = 130;
}

static void	handle_sigint(int sig)
{
	(void)sig;
	signal (SIGQUIT, SIG_IGN);
	write(1, "\n", 1);
	g_ss = 130;
}

static void	handle_sigquit(int sig)
{
	(void)sig;
	write(2, "Quit (core dumped)\n", 19);
	g_ss = 131;
}

void	signal_init(int sig)
{
	g_ss = 0;
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
