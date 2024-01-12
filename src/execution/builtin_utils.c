/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:43 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/12 14:40:44 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "libft.h"

void	handle_error_cases(t_exec *exec)
{
	if (exec->argc == 1)
	{
		if (ft_strcmp(exec->argv[0], ".") == 0)
		{
			printf("minishell: .: filename argument required\n");
			printf(".: usage: . filename [arguments]\n");
			g_exit_status = 2;
		}
		else if (ft_strcmp(exec->argv[0], "..") == 0)
		{
			printf("minishell: %s: command not found\n", exec->argv[0]);
			g_exit_status = 127;
		}
		else
		{
			printf("minishell: %s: command not found\n", exec->argv[0]);
			g_exit_status = 127;
		}
	}
}
