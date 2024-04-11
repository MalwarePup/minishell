/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:32:49 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/11 12:50:01 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

int	count_pipe(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->type == CMD_PIPE)
			i++;
		token = token->next;
	}
	return (i + 1);
}

t_token	*handle_command_error(t_master *master, t_token *token, t_cmd_type type)
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
			else if (master->exit_status == NOT_FOUND && !master->exec->pipe)
				master->exit_status = EXIT_SUCCESS;
		}
	}
}
