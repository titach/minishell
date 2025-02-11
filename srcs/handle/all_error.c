/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:37:39 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/15 23:37:39 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_status_cmd_builtins(t_phaser *sh, t_cmd *div, int i)
{
	if (i == 50)
		i = do_exit(sh, div, 2, 0);
	else if (i == 30)
		cd_cmd(sh, div, 2);
	else if (i == 20)
		export_cmd(sh, div, 2, 0);
	return (sh->exit);
}

static int	handle_status_cmd(t_phaser *sh, t_cmd *div, int i)
{
	char	*join;

	i = select_builtins(div);
	if (i == 0 || i == 200)
	{
		join = ft_strjoin("/bin/", div->command[0]);
		if (access(join, X_OK) == 0 && file_in(div->program, -1) == 1)
		{
			free(join);
			return (WEXITSTATUS(sh->status));
		}
		free(join);
		if (div->path == true && !ft_strnstr(div->command[0],
				"/", ft_strlen(div->command[0])))
			return (127);
		i = file_in(div->program, -1);
		if (i == 0 || i == 20 || i == 1)
			return (126);
		return (127);
	}
	else if (i == 50 || i == 20 || i == 30)
		return (handle_status_cmd_builtins(sh, div, i));
	return (0);
}

static int	handle_status_file(t_cmd *div, int fd)
{
	t_file	*file;

	file = div->file;
	while (file)
	{
		if (file->re == 1 || file->re == 4)
			fd = open(file->name, O_RDONLY);
		else if (file->re == 2)
			fd = open(file->name, O_CREAT | O_WRONLY, 0644);
		else if (file->re == 3)
			fd = open(file->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			return (1);
		close(fd);
		file = file->next;
	}
	return (0);
}

int	handle_all_status(t_phaser *sh, t_cmd *div, int i)
{
	div = sh->start;
	while (div->pipe)
		div = div->pipe;
	if (handle_status_file(div, i) == 1)
		return (1);
	return (handle_status_cmd(sh, div, i));
}

void	handle_all(t_phaser *sh, t_cmd *div, int i)
{
	char	*cmd[3];

	cmd[0] = "sleep";
	cmd[1] = "0";
	cmd[2] = NULL;
	i = select_builtins(div);
	if (i == 0)
	{
		if (div->path == true && !ft_strnstr(div->command[0]
				, "/", ft_strlen(div->command[0])))
		{
			msg_error(div->command[0], ": command not found\n", 20);
			execve("/bin/sleep", cmd, sh->env);
		}
		i = file_in(div->program, 0);
		if (i == 0 || i == 20 || i == 1)
		{
			if (i == 0)
				msg_error(div->command[0], ": Is a directory\n", 1);
			else if (i == 1)
				msg_error(div->command[0], ": Permission denied\n", 1);
		}
	}
	execve("/bin/sleep", cmd, sh->env);
}
