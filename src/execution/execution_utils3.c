/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 09:32:17 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/10 09:37:39 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "minishell.h"

t_token *handle_command_error(t_master *master, t_token *token, t_cmd_type type)
{
	if (type == CMD_ERROR)
	{
		if (token->next && token->next->type == CMD_PIPE)
		{
			free_string_array(&master->argv);
			token = token->next->next;
		}
		else
		{
			free_string_array(&master->argv);
			return (NULL);
		}
	}
	return (token);
}

void	wait_for_processes(t_master *master, int num_pids)
{
	int	i;
	int	status;

	i = -1;
	while (++i < num_pids)
	{
		while ((waitpid(master->pid_list[i], &status, 0)) > 0)
		{
			if (WIFEXITED(status) && master->exit_status != NOT_FOUND)
				master->exit_status = WEXITSTATUS(status);
			else if (master->exit_status == NOT_FOUND
				&& !master->exec->pipe)
				master->exit_status = EXIT_SUCCESS;
		}
	}
}
