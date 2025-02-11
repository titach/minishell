/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tchaloei <tchaloei@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:46:47 by tchaloei          #+#    #+#             */
/*   Updated: 2025/01/18 18:46:47 by tchaloei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*program_builtins(int flag)
{
	if (flag == 10)
		return (ft_strdup("/usr/bin/echo"));
	else
		return (ft_strdup("/usr/bin/pwd"));
}
