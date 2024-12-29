/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:28:41 by tchaloei          #+#    #+#             */
/*   Updated: 2024/12/23 15:28:41 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit(char *input)
{
	char	**cmd;

	cmd = ft_split(input, ' ');
	if (ft_strncmp(cmd[0], "exit", ft_strlen(cmd[0])) != 0)
	{
		ft_free_split(cmd);
		return (0);
	}
	printf("exit\n");
	if (ft_strlen2d(cmd) > 2)
		write(2, "bash: exit: too many arguments\n", 31);
	return (1);
}
