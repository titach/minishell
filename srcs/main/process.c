/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 19:10:39 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/17 19:10:39 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	create_pipe(t_phaser *sh, int j)
{
	if (j % 2 == 0)
	{
		if (pipe(sh->pipefd) == -1)
		{
			perror("pipe");
			exit(1);
		}
	}
	else
	{
		if (pipe(sh->pipefd2) == -1)
		{
			perror("pipe");
			exit(1);
		}
	}
}

static void	close_pipe(t_phaser *sh, int i, int pipefd[2], int pipefd2[2])
{
	if (sh->pipe > 1)
	{
		if (i % 2 != 0 && i > 0)
		{
			close(pipefd[0]);
			close(pipefd[1]);
		}
		else if (i % 2 == 0 && i > 0)
		{
			close(pipefd2[0]);
			close(pipefd2[1]);
		}
	}
}

static void	redirect_pipe(int fd[2], int flag)
{
	if (flag == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd[1]);
			close(fd[0]);
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
		close(fd[0]);
	}
	else
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd[1]);
			close(fd[0]);
			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		close(fd[1]);
	}
}

static void	subprocess_cons(t_phaser *sh, t_cmd *div, int i)
{
	if (sh->pipe > 1)
	{
		if (i == 0)
			redirect_pipe(sh->pipefd, 0);
		else if (i % 2 != 0 && i > 0 && i < sh->pipe - 1)
		{
			redirect_pipe(sh->pipefd, 1);
			redirect_pipe(sh->pipefd2, 0);
		}
		else if (i % 2 == 0 && i > 0 && i < sh->pipe - 1)
		{
			redirect_pipe(sh->pipefd, 0);
			redirect_pipe(sh->pipefd2, 1);
		}
		else if (i == sh->pipe - 1 && i % 2 != 0)
			redirect_pipe(sh->pipefd, 1);
		else if (i == sh->pipe - 1 && i % 2 == 0)
			redirect_pipe(sh->pipefd2, 1);
	}
	if (div->file)
		redirection_file(sh, div, 0);
	do_builtins(sh, div);
	if (execve(div->program, div->command, sh->env) == -1)
		handle_all(sh, div, 0);
}

void	start_process(t_phaser *sh, t_cmd *div, int i, int j)
{
	sh->pids = malloc(sizeof(pid_t) * sh->pipe);
	if (!sh->pids)
		error_func("malloc error");
	signal_init(2);
	while (div)
	{
		if (j < sh->pipe - 1 && sh->pipe > 1)
			create_pipe(sh, j);
		sh->pids[i] = fork();
		if (sh->pids[i] < 0)
			error_func("fork");
		else if (sh->pids[i] == 0)
			subprocess_cons(sh, div, i);
		close_pipe(sh, i++, sh->pipefd, sh->pipefd2);
		j++;
		div = div->pipe;
	}
	j = 0;
	while (j < i)
		waitpid(sh->pids[j++], &sh->status, 0);
	if (g_ss > 0)
		sh->exit = g_ss;
	else
		sh->exit = handle_all_status(sh, div, 0);
	free(sh->pids);
}
