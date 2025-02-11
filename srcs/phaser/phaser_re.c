/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phaser_re.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:01:13 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/28 23:01:13 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*file_name(char *tmp, int i, bool fc)
{
	char	*file;
	char	*flag;

	if (tmp[i] == '<' || tmp[i] == '>')
		i++;
	while (tmp[i] && tmp[i] == ' ' && fc == true)
		i++;
	flag = ft_strdup(tmp);
	file = malloc(sizeof(char) * (count_lengt(tmp, i - 1, flag) + 1));
	if (!file)
		return (NULL);
	free(flag);
	i = count_lengt(tmp, i - 1, file);
	i = -1;
	while (file[++i])
	{
		if (file[i] == 8)
			file[i] = '<';
		else if (file[i] == 9)
			file[i] = '>';
		else if (file[i] == 10)
			file[i] = ' ';
	}
	return (file);
}

void	phaser_command(char *tmp, t_cmd *div, int i)
{
	while (tmp[++i])
	{
		if (tmp[i] == '<' || tmp[i] == '>')
		{
			while (tmp[i] == '<' || tmp[i] == '>')
				tmp[i++] = ' ';
			while (tmp[i] == ' ')
				i++;
			while (tmp[i] && tmp[i] != ' ' && tmp[i] != '<' && tmp[i] != '>')
				tmp[i++] = ' ';
			i--;
		}
	}
	div->loaded = ft_split(tmp, ' ');
	div->command = malloc(sizeof(char *) * (ft_strlen2d(div->loaded) + 1));
	if (!div->command)
		return ;
	i = -1;
	while (div->loaded[++i])
		div->command[i] = file_name(div->loaded[i], 0, 0);
	div->command[i] = NULL;
	ft_free_split(div->loaded);
}

int	find_redirect(char *tmp, int *i)
{
	if (*i > 0 && tmp[*i] == '<' && tmp[(*i) - 1] == '<')
	{
		(*i)--;
		return (4);
	}
	else if (*i > 0 && tmp[*i] == '>' && tmp[(*i) - 1] == '>')
	{
		(*i)--;
		return (3);
	}
	else if (tmp[*i] == '>')
		return (2);
	else if (tmp[*i] == '<')
		return (1);
	return (0);
}

void	sep_filename(char *tmp, t_cmd *div, int i)
{
	t_file	*file;

	while (--i >= 0)
	{
		if (tmp[i] == '<' || tmp[i] == '>')
		{
			file = (t_file *)malloc(sizeof(t_file));
			if (!file)
				return ;
			file->re = find_redirect(tmp, &i);
			file->name = file_name(tmp, i + 1, 1);
			file->next = div->file;
			div->file = file;
		}
	}
}

void	check_redirect(char *tmp, t_cmd *div, int i)
{
	char	q;

	while (tmp[++i])
	{
		if (tmp[i] == 34 || tmp[i] == 39)
		{
			q = tmp[i];
			while (tmp[++i] && tmp[i] != q)
			{
				if (tmp[i] == '<' || tmp[i] == '>' || tmp[i] == ' ')
					tmp[i] = replace_imp(tmp[i]);
			}
		}
	}
	sep_filename(tmp, div, ft_strlen(tmp));
	phaser_command(tmp, div, -1);
	free(tmp);
}
