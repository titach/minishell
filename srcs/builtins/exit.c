/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:39:18 by tchaloei          #+#    #+#             */
/*   Updated: 2025/02/11 02:22:33 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	equal_zero(char *num, int i)
{
	char	**tmp;
	int		c;

	tmp = ft_split(num, ' ');
	if (tmp[1])
	{
		ft_free_split(tmp);
		return (1);
	}
	c = 0;
	while (tmp[0][++i])
	{
		if (ft_isdigit(tmp[0][0]) == 0 && tmp[0][0] != '-' && tmp[0][0] != '+')
			c = 1;
		if (i > 0 && ft_isdigit(tmp[0][i]) == 0)
			c = 1;
	}
	if ((tmp[0][0] == '-' || tmp[0][0] == '+') && !tmp[0][1])
		c = 1;
	ft_free_split(tmp);
	return (c);
}

int	reduce_num(t_phaser *sh, t_cmd *div, long long num, int flag)
{
	char	*n;

	if (flag == 0)
	{
		n = ft_itoa((int)num);
		sh->exit = num;
		free(n);
	}
	else if (flag == 1)
	{
		num = ft_atoll(div->command[1]) % 256;
		if (num < 0)
			num = num + 256;
	}
	return (num);
}

int	long_overflow(char *num, long long n)
{
	char	**tmp;
	int		flag;

	flag = 0;
	tmp = ft_split(num, ' ');
	if (n < 0 && (tmp[0][0] == '+' || ft_isdigit(tmp[0][0]) == 1))
		flag = 1;
	if (n > 0 && tmp[0][0] == '-')
		flag = 1;
	ft_free_split(tmp);
	return (flag);
}

static void	exit_op(t_phaser *sh, t_cmd *div, int flag)
{
	if (flag < 2)
		msg_error_builtins("bash: exit: ", div->command[1],
			": numeric argument required\n");
	sh->exit = 2;
}

int	do_exit(t_phaser *sh, t_cmd *div, int flag, long long num)
{
	if (flag == 0)
		printf("exit\n");
	if (div->command[1])
	{
		num = reduce_num(sh, div, num, 1);
		if ((num == 0 && equal_zero(div->command[1], -1) == 1)
			|| long_overflow(div->command[1], ft_atoll(div->command[1])) == 1)
			exit_op(sh, div, flag);
		else
		{
			if (div->command[2])
			{
				if (flag < 2)
					msg_error_builtins("bash: exit:", " ",
						"too many arguments\n");
				sh->exit = 1;
				flag = 1;
			}
			else
				num = reduce_num(sh, div, num, 0);
		}
	}
	return (flag);
}
