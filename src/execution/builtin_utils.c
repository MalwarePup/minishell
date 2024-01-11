/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:43 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 19:03:34 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "libft.h"

void	print_data_builtins(t_exec	*current)
{
	int	i;

	i = 0;
	printf("------ARGUMENTS ENVOYES EN EXECUTION-----------\n");
	printf("| %-15s | %-25s |\n", "Int ARGC", "Char **ARGV");
	printf("-----------------------------------------------\n");
	printf("| %-15d | %-25s |\n", current->argc, current->argv[i++]);
	while (current->argv[i])
	{
		printf("| %-15s | %-25s |\n", " ", current->argv[i]);
		i++;
	}
	printf("-----------------------------------------------\n\n\n");
}

void	handle_error_cases(t_master *master, t_exec *exec)
{
	if (exec->argc == 1)
	{
		if (ft_strcmp(exec->argv[0], ".") == 0)
		{
			printf("minishell: .: filename argument required\n");
			printf(".: usage: . filename [arguments]\n");
			master->exit_status = 2;
		}
		else if (ft_strcmp(exec->argv[0], "..") == 0)
		{
			printf("minishell: %s: command not found\n", exec->argv[0]);
			master->exit_status = 127;
		}
		else
		{
			printf("minishell: %s: command not found\n", exec->argv[0]);
			master->exit_status = 127;
		}
	}
}