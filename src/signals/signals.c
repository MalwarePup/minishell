/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 20:46:13 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/12 23:21:50 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

int	set_sigaction(t_master *master)
{
	master->act.sa_handler = default_handler;
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
	master->act.sa_handler = exec_handler;
	if (sigaction(SIGINT, &master->act, NULL) == -1)
	{
		perror("sigaction (set_sigaction_exec)");
		return (1);
	}
	return (0);
}

int	set_sigaction_heredoc(t_master *master)
{
	master->act.sa_handler = heredoc_handler;
	if (sigaction(SIGINT, &master->act, NULL) == -1)
	{
		perror("sigaction (set_sigaction_heredoc)");
		return (1);
	}
	return (0);
}

int	restore_sigaction(t_master *master)
{
	master->act.sa_handler = default_handler;
	if (sigaction(SIGINT, &master->act, NULL) == -1)
	{
		perror("sigaction (restore_sigaction)");
		return (1);
	}
	return (0);
}
