/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phaser_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 22:51:50 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/28 22:51:50 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_dollar_array(char *tmp, int *ds, int i, int j)
{
	char	q;

	while (tmp[++i])
	{
		if (tmp[i] == 34 || tmp[i] == 39)
		{
			q = tmp[i];
			while (tmp[++i] && tmp[i] != q)
			{
				if ((q == 34 && tmp[i] == '$' && ft_isalpha(tmp[i + 1]))
					|| (q == 34 && tmp[i] == '$' && tmp[i + 1] == '?'))
				{
					ds[j] = i;
					j += 2;
				}
			}
		}
		if ((tmp[i] == '$' && ft_isalpha(tmp[i + 1]))
			|| (tmp[i] == '$' && tmp[i + 1] == '?'))
		{
			ds[j] = i;
			j += 2;
		}
	}
	ds[j] = -1;
}

void	get_dollar_len(char *tmp, int *ds, int i)
{
	char	q;
	int		j;

	while (ds[i] > -1)
	{
		j = ds[i];
		if (tmp[++j] == 34 || tmp[j] == 39)
		{
			q = tmp[j++];
			while (tmp[j] && tmp[j] != q)
				j++;
			ds[i + 1] = j;
		}
		else
		{
			while (tmp[j] && ft_isalpha(tmp[j]))
				j++;
			if (tmp[j] == '?' && j - 1 == ds[i])
				j++;
			ds[i + 1] = j - 1;
		}
		i += 2;
	}
}

char	**do_dollar(t_phaser *sh, char *tmp, int *ds, int n)
{
	char	**envp;
	int		i;
	int		j;

	envp = malloc(sizeof(char *) * (n + 1));
	if (!envp)
		return (NULL);
	i = 0;
	j = 0;
	while (ds[i] > -1)
	{
		envp[j] = convert_ds(sh, tmp, ds[i] + 1, ds[i + 1]);
		j++;
		i += 2;
	}
	envp[j] = NULL;
	return (envp);
}

int	cut_and_join(char *tmp[4], char **envp, int *ds, int i)
{
	int	j;
	int	k;

	j = 0;
	while (ds[j] > -1)
	{
		k = ds[j];
		if (i == k)
		{
			tmp[2] = ft_strdup(envp[j++ / 2]);
			i = ds[j++] + 1;
		}
		else
		{
			tmp[2] = ft_substr(tmp[0], i, k - i);
			i = k;
		}
		tmp[3] = ft_strdup(tmp[1]);
		free(tmp[1]);
		tmp[1] = ft_strjoin(tmp[3], tmp[2]);
		free(tmp[2]);
		free(tmp[3]);
	}
	return (i);
}

char	*make_env_var(t_phaser *sh, char *sep, int n, char *tmp[4])
{
	int		*ds;
	char	**envp;

	tmp[0] = ft_strdup(sep);
	ds = malloc(sizeof(int *) * ((n * 2) + 1));
	if (!ds)
		return (NULL);
	get_dollar_array(tmp[0], ds, -1, 0);
	get_dollar_len(tmp[0], ds, 0);
	envp = do_dollar(sh, tmp[0], ds, n);
	tmp[1] = ft_strdup("\0");
	n = cut_and_join(tmp, envp, ds, 0);
	if (tmp[0][n])
	{
		tmp[2] = ft_substr(tmp[0], n, ft_strlen(tmp[0]) - n);
		tmp[3] = ft_strdup(tmp[1]);
		free(tmp[1]);
		tmp[1] = ft_strjoin(tmp[3], tmp[2]);
		free(tmp[2]);
		free(tmp[3]);
	}
	ft_free_split(envp);
	free(tmp[0]);
	free(ds);
	return (tmp[1]);
}
