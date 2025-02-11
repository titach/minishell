/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 14:56:31 by tchaloei          #+#    #+#             */
/*   Updated: 2025/02/09 03:04:23 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	select_builtins(t_cmd *cmd)
{
	if (ft_strncmp(cmd->command[0], "echo", 4) == 0
		&& ft_strlen(cmd->command[0]) == 4)
		return (10);
	else if (ft_strncmp(cmd->command[0], "pwd", 3) == 0
		&& ft_strlen(cmd->command[0]) == 3)
		return (100);
	else if (ft_strncmp(cmd->command[0], "env", 3) == 0
		&& ft_strlen(cmd->command[0]) == 3)
		return (200);
	else if (ft_strncmp(cmd->command[0], "export", 6) == 0
		&& ft_strlen(cmd->command[0]) == 6)
		return (20);
	else if (ft_strncmp(cmd->command[0], "cd", 2) == 0
		&& ft_strlen(cmd->command[0]) == 2)
		return (30);
	else if (ft_strncmp(cmd->command[0], "unset", 5) == 0
		&& ft_strlen(cmd->command[0]) == 5)
		return (40);
	else if (ft_strncmp(cmd->command[0], "exit", 4) == 0
		&& ft_strlen(cmd->command[0]) == 4)
		return (50);
	return (0);
}

void	print_export(char **str, char *tmp, int i, size_t len)
{
	char	*subs;

	while (str[++i])
	{
		if (str[i][0] == '_')
			i++;
		if (!str[i])
			break ;
		write(1, "declare -x ", 11);
		if (check_char(str[i], "=") == 1)
		{
			tmp = ft_strchr(str[i], '=') + 1;
			len = ft_strlen(str[i]) - ft_strlen(tmp);
			subs = ft_substr(str[i], 0, len);
			write (1, subs, len);
			free (subs);
			write (1, "\"", 1);
			if (*tmp)
				write (1, tmp, ft_strlen(tmp));
			write (1, "\"", 1);
		}
		else
			write(1, str[i], ft_strlen(str[i]));
		write (1, "\n", 1);
	}
}

void	do_builtins(t_phaser *sh, t_cmd *div)
{
	int	tmp;

	if (select_builtins(div) == 20)
		export_cmd(sh, div, 1, 0);
	else if (select_builtins(div) == 200)
		do_env(sh, div);
	else if (select_builtins(div) == 100)
		do_pwd(div);
	else if (select_builtins(div) == 50)
	{
		if (sh->start->pipe)
			tmp = do_exit(sh, div, 1, 0);
		else
			tmp = do_exit(sh, div, 0, 0);
	}
	else if (select_builtins(div) == 30)
		cd_cmd(sh, div, 1);
	(void)tmp;
}

void	set_path(t_phaser *sh, char *en, char *path)
{
	int	i;

	i = 0;
	if (c_cmp(en, "OLDPWD", 2) == 0 && sh->flag == 1)
		sh->flag = 0;
	while (sh->env[i])
	{
		if (c_cmp(sh->env[i], en, 1) == 0
			|| (c_cmp(sh->env[i], en, 1) == -61))
		{
			free(sh->env[i]);
			if (!path)
				sh->env[i] = ft_strdup(en);
			else
				sh->env[i] = ft_strjoin(en, path);
			break ;
		}
		i++;
	}
}

char	*get_env(t_phaser *sh, char *en)
{
	int	i;

	i = 0;
	while (sh->env[i])
	{
		if (ft_strncmp(sh->env[i], en, ft_strlen(en)) == 0)
			return (ft_strchr(sh->env[i], '=') + 1);
		i++;
	}
	if (sh->env[i] == NULL)
		return (NULL);
	return (0);
}
