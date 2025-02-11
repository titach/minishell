/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 00:05:42 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/14 00:05:42 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen2d(char **sep)
{
	int	i;

	i = 0;
	while (sep[i])
		i++;
	return (i);
}

long long	ft_atoll(const char *str)
{
	long long	result;
	long long	sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] && str[i] >= 48 && str[i] <= 57)
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	return (result *= sign);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	ft_free_struct(t_phaser *sh)
{
	t_cmd	*cmd;
	t_file	*file;

	while (sh->start)
	{
		cmd = sh->start;
		while (cmd->file)
		{
			file = cmd->file;
			if (file->re == 4)
				unlink(file->name);
			cmd->file = cmd->file->next;
			free(file->name);
			free(file);
		}
		if (cmd->program)
			free(cmd->program);
		ft_free_split(cmd->command);
		sh->start = sh->start->pipe;
		free(cmd);
	}
	ft_free_split(sh->sep);
}
