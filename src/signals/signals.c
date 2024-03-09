/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 20:46:13 by ladloff           #+#    #+#             */
/*   Updated: 2024/03/09 21:55:08 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

static int	initialize_signal_handlers(t_master *master)
{
	master->minishell_sa.sa_handler = handle_minishell_sig;
	master->minishell_sa.sa_flags = 0;
	if (sigemptyset(&master->minishell_sa.sa_mask) == -1)
	{
		perror("sigemptyset (initialize_signal_handlers)");
		return (1);
	}
	master->heredoc_sa.sa_handler = handle_heredoc_sig;
	master->heredoc_sa.sa_flags = 0;
	if (sigemptyset(&master->heredoc_sa.sa_mask) == -1)
	{
		perror("sigemptyset (initialize_signal_handlers)");
		return (1);
	}
	master->temp_sa.sa_handler = handle_temp_sig;
	master->temp_sa.sa_flags = 0;
	if (sigemptyset(&master->temp_sa.sa_mask) == -1)
	{
		perror("sigemptyset (initialize_signal_handlers)");
		return (1);
	}
	return (EXIT_SUCCESS);
}

int	set_sigaction(t_master *master)
{
	if (initialize_signal_handlers(master))
		return (1);
	if (sigaction(SIGINT, &master->minishell_sa, NULL) == -1
		|| sigaction(SIGQUIT, &master->minishell_sa, NULL) == -1)
	{
		perror("sigaction (set_sigaction)");
		return (1);
	}
	return (EXIT_SUCCESS);
}

int	set_sigaction_temp(t_master *master)
{
	master->heredoc_sa.sa_handler = handle_temp_sig;
	if (sigaction(SIGINT, &master->temp_sa, NULL) == -1)
	{
		perror("sigaction (set_sigaction_temp)");
		return (1);
	}
	return (EXIT_SUCCESS);
}

int	set_sigaction_heredoc(t_master *master)
{
	master->heredoc_sa.sa_handler = handle_heredoc_sig;
	if (sigaction(SIGINT, &master->heredoc_sa, NULL) == -1)
	{
		perror("sigaction (set_sigaction_heredoc)");
		return (1);
	}
	return (EXIT_SUCCESS);
}

int	restore_sigaction(t_master *master)
{
	if (sigaction(SIGINT, &master->minishell_sa, NULL) == -1)
	{
		perror("sigaction (restore_sigaction)");
		return (1);
	}
	return (EXIT_SUCCESS);
}
