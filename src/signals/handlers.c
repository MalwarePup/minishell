/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:59:55 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/13 00:17:24 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"

void	default_handler(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = 128 + signum;
		write(STDOUT_FILENO, "^C\n", 3);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	exec_handler(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = 128 + signum;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
	}
	else if (signum == SIGQUIT)
	{
		*g_exit_status = 128 + signum;
		write(STDOUT_FILENO, QUIT_STR, sizeof(QUIT_STR) - 1);
	}
}

void	heredoc_handler(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = 128 + signum;
		write(STDOUT_FILENO, "^C\n", 3);
		close(STDIN_FILENO);
	}
}
