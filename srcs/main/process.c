/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 19:13:25 by tchaloei          #+#    #+#             */
/*   Updated: 2024/12/18 19:13:25 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redirect_file(char *hd, char *eof)
{
	int		rd;
	int		fd;
	int		b;
	char	buf[10];

	rd = open(hd, O_RDONLY);
	fd = open(eof, O_CREAT | O_RDWR | O_TRUNC, 0644);
	b = 1;
	while (b > 0)
	{
		b = read(rd, buf, sizeof(buf));
		write(fd, buf, b);
	}
	close(rd);
	close(fd);
}

void	edit_input(char *input, t_minishell sh, int round)
{
	int			i;
	static int	j = 0;

	if (sh.h_ok < 0)
		return ;
	i = 0;
	if (round == 0)
		j = 0;
	while (input[i])
	{
		if (input[i] == '<' && input[i + 1] == '<')
		{
			input[++i] = ' ';
			redirect_file(sh.hd[j], sh.eof[j]);
			j++;
			sh.id_h = j;
		}
		i++;
	}
}

static void	main_process(char *input, char **en, t_minishell sh, int *ss)
{
	pid_t	pid;
	int		status;
	char	**command;
	char	*program;

	edit_input(input, sh, sh.id_h);
	status = 0;
	pid = fork();
	signal_init(2);
	if (pid < 0)
		error_func("fork error");
	else if (pid == 0)
	{
		sh.exit = *ss;
		command = select_builtins(input, sh);
		program = ft_strjoin("/bin/", command[0]);
		// all_error(command);
		execve(program, command, en);
			// error_msg(program, command);
	}
	else
	{
		waitpid(pid, &status, 0);
		signal_init(1);
	}
	*ss = WEXITSTATUS(status);
	// printf("%s\n", input);
}

void	sep_process(char *input, char **en, t_minishell sh, int *ss)
{
	sh.n = count_imp(input, '|');
	sh.sep = ft_split(input, '|');
	if (sh.n + 1 == ft_strlen2d(sh.sep) && sh.n > 0)
	{
		child_subprocess(sh, en, ss);
	}
	else
	{
		// printf("[main]\n\n");
		if (sh.n > 0)
			sh.n = -1;
		main_process(input, en, sh, ss);
	}
	ft_free_split(sh.sep);
}
