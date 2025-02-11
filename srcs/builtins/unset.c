/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:26:40 by djewapat          #+#    #+#             */
/*   Updated: 2025/01/28 22:34:38 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_env(char *un, t_phaser *sh)
{
	char	**temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	temp = (char **)malloc(sizeof(char *) * (ft_strlen2d(sh->env)));
	if (!temp)
		return ;
	while (sh->env[i])
	{
		if (c_cmp(sh->env[i], un, 3) == 0)
			i++;
		else
		{
			temp[j] = ft_strdup(sh->env[i]);
			j++;
			i++;
		}
	}
	temp[j] = NULL;
	ft_free_split(sh->env);
	sh->env = temp;
}

void	unset_cmd(t_phaser *sh, t_cmd *div)
{
	int	i;
	int	j;

	if (!div->command[1])
		return ;
	i = 1;
	while (div->command[i])
	{
		j = 0;
		if (c_cmp(div->command[i], "PWD", 3) == 0)
			sh->flag = 1;
		while (sh->env[j])
		{
			if (c_cmp(sh->env[j], div->command[i], 3) == 0)
			{
				unset_env(div->command[i], sh);
				break ;
			}
			j++;
		}
		i++;
	}
}
