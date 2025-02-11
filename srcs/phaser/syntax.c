/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:47:01 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/18 17:47:01 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_condition(t_phaser *sh, char **tmp, int i, int flag)
{
	while (tmp[++i])
	{
		if ((i == 0 && tmp[i][0] == '|') || (tmp[i][0] == '|' && !tmp[i + 1])
			|| (tmp[i][0] == '|' && tmp[i + 1][0] == '|'))
		{
			unexpected_msg(tmp[i], sh);
			flag = 1;
			break ;
		}
		if ((tmp[i][0] == '<' || tmp[i][0] == '>')
			&& token_redirect(tmp, i, -1) == 0)
		{
			if (tmp[i][1] == '|' || (tmp[i][0] == '<' && tmp[i][1] == '>')
				|| (tmp[i][0] == '>' && tmp[i][1] == '<'))
				unexpected_token(tmp[i], 0, 1);
			else if (!tmp[i + 1])
				unexpected_token(tmp[i], 0, -1);
			else
				unexpected_token(tmp[i + 1], 0, 0);
			sh->exit = 2;
			flag = 1;
			break ;
		}
	}
	return (flag);
}

int	unexpected_pipe(char *input, t_phaser *sh, int i)
{
	char	q;
	char	**tmp;
	int		flag;

	flag = 0;
	while (input[++i])
	{
		if (input[i] == 34 || input[i] == 39)
		{
			q = input[i];
			while (input[++i] && input[i] != q)
			{
				if (input[i] == ' ' || input[i] == '|'
					|| input[i] == '<' || input[i] == '>')
					input[i] = 9;
			}
		}
	}
	tmp = ft_split(input, ' ');
	flag = token_condition(sh, tmp, -1, 0);
	free(input);
	ft_free_split(tmp);
	return (flag);
}

int	unexpected_rest(char *input, t_phaser *sh, int i)
{
	int	flag;

	flag = 0;
	while (input[++i])
	{
		flag = unexpected_err(input[i], sh);
		if (flag == 1)
			break ;
	}
	free(input);
	return (flag);
}

int	semi_bslash(char *input, t_phaser *sh, int i)
{
	char	q;

	while (input[++i])
	{
		if (input[i] == 34 || input[i] == 39)
		{
			q = input[i];
			while (input[++i] && input[i] != q)
			{
				if (input[i] == ';' || input[i] == '(' || input[i] == ')'
					|| input[i] == '*' || input[i] == '&')
					input[i] = 9;
			}
		}
	}
	return (unexpected_rest(input, sh, -1));
}

int	check_syntax(char *input, t_phaser *sh, int i)
{
	char	q;

	while (input[++i] && input[i])
	{
		if (input[i] == 34 || input[i] == 39)
		{
			q = input[i];
			i++;
			while (input[i] && input[i] != q)
				i++;
			if (!input[i])
			{
				write(2, "Syntax error: Unterminated quoted string\n", 41);
				sh->exit = 1;
				return (1);
			}
		}
	}
	if (semi_bslash(ft_strdup(input), sh, -1) == 1)
		return (1);
	if (unexpected_pipe(ft_strdup(input), sh, -1) == 1)
		return (1);
	return (0);
}
