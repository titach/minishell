/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minshell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:18:27 by tchaloei          #+#    #+#             */
/*   Updated: 2024/12/18 16:18:27 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <termios.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_minishell
{
	int	i;
	int	j;
	int	n;
	int	id_h;
	int	h_ok;
	int	exit;
	int	pipefd[2];
	int	pipefd2[2];
	char *sub;
	char **eof;
	char **hd;
	char **sep;
}	t_minishell;

//process
void	sep_process(char *input, char **en, t_minishell sh, int *ss);
void	edit_input(char *input, t_minishell sh, int round);
//system
void	child_subprocess(t_minishell sh, char **en, int *ss);
//sub_system
void	subprocess_cons(t_minishell sh, char **en);
//heredoc
void	build_hd(char *input, int c, char **eof, char **file);
//error_func
void	error_func(char *msg);
void	msg_error(char *file, char *msg);
void	all_error(char **command);
//utils
void	ft_free_split(char **split);
int	count_imp(char *input, char c);
int	ft_strlen2d(char **sep);
int	count_end(char *input, int p);
char	*get_end(char *input, int j, int start);
//rules
char	**select_builtins(char *input, t_minishell sh);
//signal
void	sigint_handle(int sig);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	signal_init(int sig);
//file_err
int	redirect_err_file(char *input);

//exit
int	handle_exit(char *input);
#endif