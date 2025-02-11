/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:52:15 by tchaloei          #+#    #+#             */
/*   Updated: 2025/02/11 02:24:01 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_cd_error(char *path, int i, t_phaser *sh, int flag)
{
	if (flag == 1)
	{
		if (i == 1)
			msg_error_builtins("bash: cd: ",
				path, ": No such file or directory\n");
		else if (i == 2)
			msg_error_builtins("bash: ", "cd:",
				" too many arguments\n");
		else if (i == 3)
			msg_error_builtins("bash: cd: ", path,
				": Permission denied\n");
		else if (i == 4)
			msg_error_builtins("bash: cd: ",
				path, ": Not a directory\n");
	}
	sh->exit = 1;
	return (1);
}

static int	do_chdir(char *input, t_phaser *sh, int flag)
{
	struct stat	path_stat;
	char		*path;

	if (access(input, F_OK) == -1)
		return (ms_cd_error(input, 1, sh, flag));
	if (stat(input, &path_stat) == -1)
		return (ms_cd_error(input, 1, sh, flag));
	if (!S_ISDIR(path_stat.st_mode))
		return (ms_cd_error(input, 4, sh, flag));
	if (access(input, X_OK) == -1)
		return (ms_cd_error(input, 3, sh, flag));
	if (flag == 0)
	{
		chdir(input);
		path = getcwd(NULL, 0);
		set_path(sh, "PWD=", path);
		free (path);
	}
	return (0);
}

static int	sep_cd(t_phaser *sh, t_cmd *div, char *oldpwd, int flag)
{
	char	*en;

	sh->exit = 0;
	en = NULL;
	if (!div->command[1] || c_cmp(div->command[1], "~", 2) == 0)
	{
		en = get_env(sh, "HOME");
		return (do_chdir(en, sh, flag));
	}
	else if (c_cmp(div->command[1], "-", 2) == 0)
	{
		oldpwd = get_env(sh, "OLDPWD");
		if (flag == 0)
			printf("%s\n", oldpwd);
		return (do_chdir(oldpwd, sh, flag));
	}
	else
		return (do_chdir(div->command[1], sh, flag));
}

void	set_old_pwd(t_phaser *sh, char *newpwd, int flag)
{
	if (flag == 0)
	{
		if (sh->flag == 1)
			set_path(sh, "OLDPWD", 0);
		else
			set_path(sh, "OLDPWD=", newpwd);
	}
}

void	cd_cmd(t_phaser *sh, t_cmd *div, int flag)
{
	char		*newpwd;

	newpwd = NULL;
	if (div->command[0] && div->command[1] && div->command[2])
		ms_cd_error("", 2, sh, flag);
	else
	{
		newpwd = getcwd(NULL, 0);
		if (!newpwd)
			return ;
		if (sep_cd(sh, div, NULL, flag) == 0)
			set_old_pwd(sh, newpwd, flag);
		free(newpwd);
	}
}
