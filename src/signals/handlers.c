/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:59:55 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/12 23:21:50 by ladloff          ###   ########.fr       */
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
		*g_exit_status = INTERRUPTED;
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "^C\n", 3);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	heredoc_handler(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = INTERRUPTED_HEREDOC;
		write(STDOUT_FILENO, "^C\n", 3);
		close(STDIN_FILENO);
	}
}

void	exec_handler(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = INTERRUPTED_TEMP;
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
	}
}
