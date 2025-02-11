/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 02:29:16 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/31 02:29:16 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_in(char *name, int flag)
{
	struct stat	file;
	char		*tmp[2];
	DIR			*dir;

	if (stat(name, &file) == -1 || access(name, R_OK) == -1)
	{
		tmp[0] = ft_strjoin(strerror(errno), "\n");
		tmp[1] = ft_strjoin(": ", tmp[0]);
		free(tmp[0]);
		if (flag == 0)
			msg_error(name, tmp[1], 1);
		free(tmp[1]);
		return (errno);
	}
	dir = NULL;
	dir = opendir(name);
	if (!dir)
		return (1);
	closedir(dir);
	return (0);
}

static void	divide_file(t_file *in, t_file *out, int fd)
{
	if (in)
	{
		if (in->re == 1 || in->re == 4)
		{
			fd = open(in->name, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (out)
	{
		if (out->re == 2)
		{
			fd = open(out->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (out->re == 3)
		{
			fd = open(out->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

static void	redirection_file_err(t_phaser *sh, t_file *file)
{
	char	*cmd[3];

	cmd[0] = "sleep";
	cmd[1] = "0";
	cmd[2] = NULL;
	write(2, "bash: ", 6);
	perror(file->name);
	execve("/bin/sleep", cmd, sh->env);
}

void	redirection_file(t_phaser *sh, t_cmd *div, int fd)
{
	t_file	*file;
	t_file	*in;
	t_file	*out;

	file = div->file;
	in = NULL;
	out = NULL;
	while (file)
	{
		if (file->re == 1 || file->re == 4)
			fd = open(file->name, O_RDONLY);
		else if (file->re == 2)
			fd = open(file->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (file->re == 3)
			fd = open(file->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			redirection_file_err(sh, file);
		if (file->re == 1 || file->re == 4)
			in = file;
		else if (file->re == 2 || file->re == 3)
			out = file;
		close(fd);
		file = file->next;
	}
	divide_file(in, out, 0);
}

char	*select_path(t_cmd *div, char **en, char *cmd, int i)
{
	char	**path;
	char	*tmp[2];

	while (*en && ft_strncmp(*en, "PATH=", 5) != 0)
		en++;
	if (!*en)
		return (ft_strdup(cmd));
	path = ft_split(*en + 5, ':');
	while (path[++i])
	{
		if (ft_strnstr(path[i], "/usr/bin", ft_strlen(path[i])) == 0
			|| ft_strnstr(path[i], "/bin", ft_strlen(path[i])) == 0)
			div->path = true;
		tmp[0] = ft_strjoin(path[i], "/");
		tmp[1] = ft_strjoin(tmp[0], cmd);
		free(tmp[0]);
		if (access(tmp[1], X_OK) == 0)
		{
			ft_free_split(path);
			return (tmp[1]);
		}
		free(tmp[1]);
	}
	ft_free_split(path);
	return (ft_strdup(cmd));
}
