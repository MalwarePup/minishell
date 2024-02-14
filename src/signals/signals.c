/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 20:46:13 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/14 13:04:25 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

static void	initialize_signal_handlers(t_master *master)
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
}

void	set_sigaction(t_master *master)
{
	initialize_signal_handlers(master);
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
