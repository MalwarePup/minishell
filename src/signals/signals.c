/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 20:46:13 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/12 23:18:23 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

int	set_sigaction(t_master *master)
{
	master->act.sa_handler = handle_minishell_sig;
	if (sigemptyset(&master->act.sa_mask) == -1)
	{
		perror("sigemptyset (set_sigaction)");
		return (1);
	}
	if (sigaction(SIGINT, &master->act, NULL) == -1
		|| sigaction(SIGQUIT, &master->act, NULL) == -1)
	{
		perror("sigaction (set_sigaction)");
		return (1);
	}
	return (0);
}

int	set_sigaction_exec(t_master *master)
{
	master->act.sa_handler = handle_temp_sig;
	if (sigaction(SIGINT, &master->act, NULL) == -1)
	{
		perror("sigaction (set_sigaction_exec)");
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
