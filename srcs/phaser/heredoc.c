/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 01:34:39 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/18 01:34:39 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	receive_input(char *eof, char *file)
{
	int		fd;
	size_t	len;
	char	*text;
	char	*input;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		error_func("can not open file.");
	len = ft_strlen(eof);
	while (1)
	{
		input = readline("> ");
		if (ft_strlen(input) > len)
			len = ft_strlen(input);
		if (ft_strncmp(input, eof, len) == 0)
		{
			free(input);
			break ;
		}
		text = ft_strjoin(input, "\n");
		write(fd, text, ft_strlen(text));
		free(text);
		free(input);
	}
	close(fd);
}

void	do_heredoc(t_phaser *sh, t_cmd *div, t_file *file, int i)
{
	char	*tmp;
	char	*hd_file;

	while (div)
	{
		file = div->file;
		while (file)
		{
			if (file->re == 4)
			{
				tmp = ft_itoa(i);
				hd_file = ft_strjoin("/tmp/heredoc", tmp);
				free(tmp);
				receive_input(file->name, hd_file);
				free(file->name);
				file->name = ft_strdup(hd_file);
				free(hd_file);
				i++;
			}
			file = file->next;
		}
		div = div->pipe;
	}
	sh->hd = i;
}

void	null_cmd(t_cmd *div)
{
	div->program = ft_strdup("/bin/sleep");
	ft_free_split(div->command);
	div->command = malloc(sizeof(char *) * 3);
	div->command[0] = ft_strdup("sleep");
	div->command[1] = ft_strdup("0");
	div->command[2] = NULL;
}
