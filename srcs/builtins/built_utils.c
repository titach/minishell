/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:26:33 by djewapat          #+#    #+#             */
/*   Updated: 2025/02/11 02:07:29 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	c_cmp(char *input, char *cmd, int i)
{
	int	result;

	result = 0;
	if (i == 1)
		result = ft_strncmp(input, cmd, ft_strlen(cmd));
	else if (i == 2)
		result = ft_strncmp(input, cmd, ft_strlen(cmd) + 1);
	else if (i == 3)
	{
		if (ft_strncmp(input, cmd, ft_strlen(cmd)) == 0
			&& (ft_strncmp(input, cmd, ft_strlen(cmd) + 1) == 0
				|| ft_strncmp(input, cmd, ft_strlen(cmd) + 1) == 61))
			return (0);
		else
			return (1);
	}
	return (result);
}

int	check_char(char *cmd, char *c)
{
	if (ft_strnstr(cmd, c, ft_strlen(cmd)) == NULL)
		return (0);
	return (1);
}

void	add_new_env(t_phaser *sh, char *cmd)
{
	char	**temp;
	int		i;

	i = 0;
	temp = (char **)malloc(sizeof(char *) * (ft_strlen2d(sh->env) + 2));
	while (sh->env[i])
	{
		temp[i] = ft_strdup(sh->env[i]);
		i++;
	}
	temp[i] = ft_strdup(cmd);
	temp[i + 1] = NULL;
	ft_free_split(sh->env);
	sh->env = temp;
}

static char	**set_str(char **str, t_phaser *sh, int i)
{
	i = 0;
	while (sh->env[i])
		i++;
	str = malloc(sizeof(char *) * (i + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (sh->env[i])
	{
		str[i] = ft_strdup(sh->env[i]);
		if (!str)
		{
			ft_free_split(str);
			return (NULL);
		}
		i++;
	}
	str[i] = NULL;
	return (str);
}

void	a_to_z(char **str, t_phaser *sh, int i, int j)
{
	char	*tmp;

	str = set_str(str, sh, 0);
	while (str[i])
	{
		j = i + 1;
		while (str[j])
		{
			if (ft_strncmp(str[i], str[j], ft_strlen(str[i])) > 0)
			{
				tmp = str[i];
				str[i] = str[j];
				str[j] = tmp;
			}
			j++;
		}
		i++;
	}
	print_export(str, tmp, -1, 0);
	sh->exit = 0;
	ft_free_split(str);
}
