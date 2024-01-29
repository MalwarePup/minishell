/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:43 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/13 18:07:25 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"

void	handle_error_cases(t_exec *exec)
{
	if (exec->argc == 1)
	{
		if (!ft_strcmp(exec->argv[0], "."))
		{
			ft_dprintf(STDERR_FILENO, ESTR_DOT_P1 ESTR_DOT_P2);
			g_exit_status = EXIT_MISUSE;
		}
		else
		{
			ft_dprintf(STDERR_FILENO, ESTR_CMD_NOT_FOUND, exec->argv[0]);
			g_exit_status = EXIT_NOT_FOUND;
		}
	}
}
