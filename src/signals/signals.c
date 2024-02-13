/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 20:46:13 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/13 14:30:28 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"

static void	handle_minishell_sig(int signum)
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

static void	handle_heredoc_sig(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = 131;
		write(STDOUT_FILENO, "^C\n", 3);
		close(STDIN_FILENO);
	}
}

static void	handle_temp_sig(int signum)
{
	if (signum == SIGINT)
	{
		*g_exit_status = 132;
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
	}
}

void	set_sigaction(t_master *master)
{
	master->minishell_sa.sa_handler = handle_minishell_sig;
	master->minishell_sa.sa_flags = 0;
	if (sigemptyset(&master->minishell_sa.sa_mask) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	master->heredoc_sa.sa_handler = handle_heredoc_sig;
	master->heredoc_sa.sa_flags = 0;
	if (sigemptyset(&master->heredoc_sa.sa_mask) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	master->temp_sa.sa_handler = handle_temp_sig;
	master->temp_sa.sa_flags = 0;
	if (sigemptyset(&master->temp_sa.sa_mask) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGINT, &master->minishell_sa, NULL) == -1
		|| sigaction(SIGQUIT, &master->minishell_sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

void	set_sigaction_temp(t_master *master)
{
	master->heredoc_sa.sa_handler = handle_temp_sig;
	if (sigaction(SIGINT, &master->temp_sa, NULL) == -1)
		error_exit(master, "sigaction (set_sigaction_heredoc)", false);
}

void	set_sigaction_heredoc(t_master *master)
{
	master->heredoc_sa.sa_handler = handle_heredoc_sig;
	if (sigaction(SIGINT, &master->heredoc_sa, NULL) == -1)
		error_exit(master, "sigaction (set_sigaction_heredoc)", false);
}

void	restore_sigaction(t_master *master)
{
	if (sigaction(SIGINT, &master->minishell_sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}
