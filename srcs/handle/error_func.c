/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 23:18:36 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/15 23:18:36 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_func(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

void	msg_error(char *file, char *msg, int flag)
{
	if (flag == 1)
		write(2, "bash: ", 6);
	write(2, file, ft_strlen(file));
	write(2, msg, ft_strlen(msg));
}

void	msg_error_builtins(char *bash, char *mid, char *msg)
{
	write(2, bash, ft_strlen(bash));
	write(2, mid, ft_strlen(mid));
	write(2, msg, ft_strlen(msg));
}
