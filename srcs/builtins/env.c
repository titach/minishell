/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:36:29 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/19 00:36:29 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_env(t_phaser *sh, t_cmd *div)
{
	int	i;

	i = 0;
	if (div->command[1])
		return ;
	while (sh->env[i] && !div->command[1])
	{
		write(1, sh->env[i], ft_strlen(sh->env[i]));
		write(1, "\n", 1);
		i++;
	}
	ft_free_split(div->command);
	free(div->program);
	div->program = "/bin/sleep";
	div->command = malloc(sizeof(char *) * 3);
	div->command[0] = "sleep";
	div->command[1] = "0";
	div->command[2] = NULL;
}

void	do_pwd(t_cmd *div)
{
	if (div->command[1])
	{
		ft_free_split(div->command);
		div->command = malloc(sizeof(char *) * 2);
		div->command[0] = "pwd";
		div->command[1] = NULL;
	}
}

int	close_shell(t_cmd *div, int flag, long long num)
{
	if (div->command[1])
	{
		num = ft_atoll(div->command[1]) % 256;
		if (num < 0)
			num = num + 256;
		if ((num == 0 && equal_zero(div->command[1], -1) == 1)
			|| ft_atoll(div->command[1]) <= LLONG_MIN)
			flag = 0;
		else
		{
			if (div->command[2])
				flag = 1;
		}
	}
	return (flag);
}
