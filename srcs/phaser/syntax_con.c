/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_con.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:36:24 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/27 22:36:24 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unexpected_msg(char *input, t_phaser *sh)
{
	write(2, "bash: syntax error near unexpected token `", 42);
	write(2, input, ft_strlen(input));
	write(2, "'\n", 2);
	sh->exit = 2;
	return (1);
}

int	unexpected_err(char input, t_phaser *sh)
{
	char	tmp[2];

	tmp[0] = input;
	tmp[1] = '\0';
	if (input == ';')
		return (unexpected_msg(tmp, sh));
	else if (input == 92)
		return (unexpected_msg(tmp, sh));
	else if (input == '*')
		return (unexpected_msg(tmp, sh));
	else if (input == '&')
		return (unexpected_msg(tmp, sh));
	else if (input == '(')
		return (unexpected_msg(tmp, sh));
	else if (input == ')')
		return (unexpected_msg(tmp, sh));
	return (0);
}

void	unexpected_token(char *next, size_t i, int flag)
{
	size_t	len;

	if (flag == 1)
		next = next + 1;
	len = ft_strlen(next);
	write(2, "bash: syntax error near unexpected token `", 42);
	if (flag == -1 && len < 3)
		write(2, "newline", 7);
	else
	{
		if (flag == -1)
			len = len - 2;
		while (i < len && i < 3)
		{
			write(2, &next[0], 1);
			i++;
		}
	}
	write(2, "'\n", 2);
	if (flag == 1)
		next = next - 1;
}

static int	return_re(char *re)
{
	if (re[0] == '<' && re[1] == '<' && re[2] == '<' && re[3] == '\0')
		return (1);
	else if (re[0] == '<' && re[1] == '<' && re[2] == '\0')
		return (1);
	else if (re[0] == '>' && re[1] == '>' && re[2] == '\0')
		return (1);
	else if (re[0] == '<' && re[1] == '\0')
		return (1);
	else if (re[0] == '>' && re[1] == '\0')
		return (1);
	return (0);
}

int	token_redirect(char **re, int p, int i)
{
	while (re[p][++i])
	{
		if (re[p][i] != '<' && re[p][i] != '>' && re[p][i] != '|')
			return (1);
	}
	if (return_re(re[p]) == 1)
	{
		if (re[p + 1] && re[p + 1][0] != '<' && re[p + 1][0] != '>'
			&& re[p + 1][0] != '|')
			return (1);
	}
	return (0);
}
