/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 20:46:13 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/12 23:11:28 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

static int	initialize_signal_handlers(t_master *master)
{
	master->act.sa_handler = handle_minishell_sig;
	if (sigemptyset(&master->act.sa_mask) == -1)
	{
		perror("sigemptyset (initialize_signal_handlers)");
		return (1);
	}
	return (0);
}

int	set_sigaction(t_master *master)
{
	if (initialize_signal_handlers(master))
		return (1);
	if (sigaction(SIGINT, &master->act, NULL) == -1
		|| sigaction(SIGQUIT, &master->act, NULL) == -1)
	{
		perror("sigaction (set_sigaction)");
		return (1);
	}
	return (0);
}

int	set_sigaction_temp(t_master *master)
{
	master->act.sa_handler = handle_temp_sig;
	if (sigaction(SIGINT, &master->act, NULL) == -1)
	{
		perror("sigaction (set_sigaction_temp)");
		return (1);
	}
	return (0);
}

int	set_sigaction_heredoc(t_master *master)
{
	master->act.sa_handler = handle_heredoc_sig;
	if (sigaction(SIGINT, &master->act, NULL) == -1)
	{
		perror("sigaction (set_sigaction_heredoc)");
		return (1);
	}
	return (0);
}

int	restore_sigaction(t_master *master)
{
	master->act.sa_handler = handle_minishell_sig;
	if (sigaction(SIGINT, &master->act, NULL) == -1)
	{
		perror("sigaction (restore_sigaction)");
		return (1);
	}
	return (0);
}
