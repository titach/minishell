/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phaser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:03:25 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/28 23:03:25 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	replace_imp(char m)
{
	if (m == '<')
		return (8);
	else if (m == '>')
		return (9);
	return (10);
}

static int	last_string(char end)
{
	if (end == ' ' || end == '<' || end == '>')
		return (1);
	return (0);
}

int	count_lengt(char *tmp, int i, char *file)
{
	int		count;
	char	q;

	count = 0;
	while (tmp[++i] && last_string(tmp[i]) == 0)
	{
		if (tmp[i] == 34 || tmp[i] == 39)
		{
			q = tmp[i];
			while (tmp[++i] && tmp[i] != q)
			{
				file[count] = tmp[i];
				count++;
			}
		}
		else
		{
			file[count] = tmp[i];
			count++;
		}
	}
	file[count] = '\0';
	return (count);
}

static int	clone_envp(char *env, char *ep, size_t len)
{
	char	**tmp;

	tmp = ft_split(env, '=');
	if (ft_strncmp(tmp[0], ep, len) == 0 && ft_strlen(tmp[0]) == len)
	{
		ft_free_split(tmp);
		return (1);
	}
	ft_free_split(tmp);
	return (0);
}

char	*convert_ds(t_phaser *sh, char *tmp, int s, int end)
{
	char	*ep[2];
	size_t	l;

	ep[0] = NULL;
	if (tmp[s] == 34 || tmp[s++] == 39)
		ep[0] = ft_substr(tmp, s, end - s);
	else
	{
		ep[1] = ft_substr(tmp, s - 1, end - s + 2);
		if (tmp[s - 1] == '?')
			ep[0] = ft_itoa(sh->exit);
		else
		{
			s = 0;
			l = ft_strlen(ep[1]);
			while (sh->env[s] && clone_envp(sh->env[s], ep[1], l) == 0)
				s++;
			if (sh->env[s])
				ep[0] = ft_substr(sh->env[s], l + 1, ft_strlen(sh->env[s]) - l);
			else
				ep[0] = ft_strdup("\0");
		}
		free(ep[1]);
	}
	return (ep[0]);
}
