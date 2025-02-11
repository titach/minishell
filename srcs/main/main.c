/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 00:19:07 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/11 00:19:07 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ss;

int	builtins_con(t_phaser *sh)
{
	int	shell_exit;

	shell_exit = 1;
	if (select_builtins(sh->start) == 20 && !sh->start->pipe)
		export_cmd(sh, sh->start, 0, 0);
	else if (select_builtins(sh->start) == 30 && !sh->start->pipe)
		cd_cmd(sh, sh->start, 0);
	else if (select_builtins(sh->start) == 40 && !sh->start->pipe)
		unset_cmd(sh, sh->start);
	else if (select_builtins(sh->start) == 50 && !sh->start->pipe)
		shell_exit = close_shell(sh->start, 0, 0);
	return (shell_exit);
}

int	loading_process(char *input, t_phaser *sh)
{
	int	shell_exit;

	shell_exit = 1;
	sh->input = input;
	start_phaser(input, sh, 0);
	start_process(sh, sh->start, 0, 0);
	signal_init(1);
	if (!sh->start->pipe && sh->start->command[0])
		shell_exit = builtins_con(sh);
	ft_free_struct(sh);
	if (shell_exit == 0)
		free(input);
	return (shell_exit);
}

void	prompt_line(t_phaser *sh)
{
	char	*input;

	signal_init(1);
	while (1)
	{
		input = readline("\1\e[1;36m\2minishell > \1\e[0m\2");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (g_ss > 0)
			sh->exit = g_ss;
		if (input && *input)
		{
			add_history(input);
			if (check_syntax(input, sh, -1) == 0)
			{
				if (loading_process(input, sh) == 0)
					break ;
			}
		}
		free(input);
	}
}

void	get_shell_data(t_phaser *sh, char **en, int e)
{
	sh->start = NULL;
	sh->pids = NULL;
	sh->sep = NULL;
	sh->input = NULL;
	sh->tmp = NULL;
	sh->start = 0;
	sh->stop = 0;
	sh->pipe = 0;
	sh->exit = 0;
	sh->hd = 0;
	sh->pipefd[0] = 0;
	sh->pipefd[1] = 0;
	sh->pipefd2[0] = 0;
	sh->pipefd2[1] = 0;
	sh->flag = 0;
	sh->env = (char **)malloc(sizeof(char *) * (ft_strlen2d(en) + 1));
	if (!sh->env)
		exit(1);
	while (en[++e])
		sh->env[e] = ft_strdup(en[e]);
	sh->env[e] = NULL;
}

int	main(int ac, char **av, char **en)
{
	t_phaser	*sh;
	int			status;

	(void)ac;
	(void)av;
	status = 0;
	sh = (t_phaser *)malloc(sizeof(t_phaser));
	if (!sh)
		exit(1);
	get_shell_data(sh, en, -1);
	prompt_line(sh);
	clear_history();
	status = sh->exit;
	ft_free_split(sh->env);
	free(sh);
	return (status);
}
