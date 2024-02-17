/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:59:55 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/17 13:56:14 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"

void	handle_minishell_sig(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = EXIT_INTERRUPTED;
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "^C\n", 3);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_heredoc_sig(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = EXIT_INTERRUPTED_HEREDOC;
		write(STDOUT_FILENO, "^C\n", 3);
		close(STDIN_FILENO);
	}
}

void	handle_temp_sig(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = EXIT_INTERRUPTED_TEMP;
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
	}
}
