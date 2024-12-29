/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:17:33 by tchaloei          #+#    #+#             */
/*   Updated: 2024/12/23 15:17:33 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	put_exit_ss2(char *new, char *old, char *status)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	while (old[i])
	{
		if (old[i] == '$' && old[++i] == '?')
		{
			j = 0;
			while (status[j])
			{
				new[k++] = status[j];
				j++;
			}
			k--;
		}
		else
			new[k] = old[i];
		k++;
		i++;
	}
	new[k] = '\0';
}

static int	find_exit_imp(char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
			j++;
		i++;
	}
	return (j);
}

static void	handle_exit_ss(char *input, t_minishell sh)
{
	char	*status;
	char	*old;

	if (sh.exit == 0)
		return ;
	status = ft_itoa(sh.exit);
	old = ft_strdup(input);
	if (sh.exit > 9 && sh.exit < 100)
		put_exit_ss2(input, old, status);
	else if (sh.exit < 10)
	{
		free(input);
		input = malloc(sizeof(char) * (ft_strlen(old) + find_exit_imp(input) + 1));
		put_exit_ss2(input, old, status);
	}
	else if (sh.exit > 99)
	{
		free(input);
		input = malloc(sizeof(char) * (ft_strlen(old) + find_exit_imp(input) + 1));
		put_exit_ss2(input, old, status);
	}
	free(status);
	free(old);
}

char	**select_builtins(char *input, t_minishell sh)
{
	char	**command;

	if (ft_strnstr(input, "<", sizeof(input)) || ft_strnstr(input, ">", sizeof(input)))
	{
		if (redirect_err_file(input) == 1)// not include envariable ( $HOME $$ $? )
		{
			free(input);
			input = ft_strdup("cp 2> /dev/null");
		}
	}
	// (void)sh;
	// if (sh.n < 0) //ถ้าอยู่ใน "' ไม่ต้องนับ
	// {
	// 	free(input);
	// 	input = ft_strdup("ls a 2> /dev/null");
	// 	write(2, "bash: syntax error near unexpected token `|'\n", 45);
	// }
	if (ft_strnstr(input, "echo", ft_strlen(input)))
		handle_exit_ss(input, sh);
	command = (char **)malloc(sizeof(char *) * 4);
	command[0] = "sh";
	command[1] = "-c";
	command[2] = input;
	command[3] = NULL;
	return (command);
}
