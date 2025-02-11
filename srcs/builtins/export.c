/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:54:32 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/19 00:54:32 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_error_export(char *cmd, int i)
{
	if (ft_isalpha(cmd[0]) == 0)
		return (1);
	if (check_char(cmd, "=") == 0 && cmd[ft_strlen(cmd) - 1] == '-')
		return (1);
	i = 1;
	while (cmd[i] && cmd[i] != '=')
	{
		if (cmd[i] == '+' && cmd[i + 1] == '=')
			i++;
		else if (ft_isalpha(cmd[i]) == 0
			&& ft_isdigit(cmd[i]) == 0 && cmd[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

static char	*set_new_cmd(char *cmd, int i)
{
	while (cmd[i])
	{
		if (cmd[i] == '+' && cmd[i + 1] == '=')
		{
			while (cmd[i])
			{
				cmd[i] = cmd[i + 1];
				i++;
			}
			break ;
		}
		i++;
	}
	return (cmd);
}

void	do_export_cmd(t_phaser *sh, char *cmd, int i, char **temp)
{
	temp = ft_split(cmd, '=');
	while (sh->env[i])
	{
		if (c_cmp(sh->env[i], temp[0], 3) == 0)
		{
			if (check_char(sh->env[i], "=") == 1 && check_char(cmd, "=") == 0)
			{
				free(sh->env[i]);
				sh->env[i] = ft_strjoin(temp[0], "=");
			}
			else
			{
				free(sh->env[i]);
				sh->env[i] = ft_strdup(cmd);
			}
			break ;
		}
		i++;
	}
	if (sh->env[i] == NULL)
		add_new_env(sh, cmd);
	ft_free_split(temp);
}

void	msg_error_export(char *cmd, t_phaser *sh, int flag)
{
	if (flag == 1)
		msg_error_builtins("bash: export: `",
			cmd, "': not a valid identifier\n");
	sh->exit = 1;
}

void	export_cmd(t_phaser *sh, t_cmd *div, int flag, int i)
{
	if (div->command[1] == NULL && flag == 1)
		a_to_z(NULL, sh, 0, 0);
	else if (div->command[1] != NULL)
	{
		i = 1;
		while (div->command[i])
		{
			if (div->command[i] != NULL)
			{
				if (check_error_export(div->command[i], 0) == 1)
					msg_error_export(div->command[i], sh, flag);
				else if (flag == 0)
				{
					if (ft_strnstr(div->command[i], "+=",
							ft_strlen(div->command[i])) != NULL)
						div->command[i] = set_new_cmd(div->command[i], 0);
					do_export_cmd(sh, div->command[i], 0, NULL);
				}
			}
			i++;
		}
	}
}
