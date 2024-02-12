/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:20:24 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/12 17:21:32 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"

static t_cmd_type	prepare_execution(t_master *master, t_token *token)
{
	t_cmd_type	type;

	if (token->type < CMD_RED_IN)
	{
		type = preparation_args(master, token);
		if (token->data
			&& ((master->exit_status == EXIT_NOT_FOUND
					|| master->exit_status == EXIT_CANNOT_EXECUTE)
				|| (!token->next && (type >= CMD_CD && type <= CMD_EXPORT))))
		{
			if (!token->next && (type >= CMD_CD && type <= CMD_EXPORT))
				launch_builtin(master, type, token);
			return (cleanup_executable(master), CMD_ERROR);
		}
	}
	else
		type = token->type;
	creation_pipe(master, token);
	return (type);
}

static void	child_process_execution(t_master *master, t_token *token,
			t_cmd_type type)
{
	if (master->exit_status != 127 && master->exec->pid == 0)
	{
		if (master->exec->first_cmd == 0)
		{
			dup2(master->exec->old_pipefd[0], STDIN_FILENO);
			close(master->exec->old_pipefd[0]);
			close(master->exec->old_pipefd[1]);
		}
		if (token->next && token->next->type == CMD_PIPE)
		{
			dup2(master->exec->pipefd[1], STDOUT_FILENO);
			close(master->exec->pipefd[0]);
			close(master->exec->pipefd[1]);
		}
		launch_redirection(master, token->redir);
		if (token->data)
			chose_execute(master, type);
		if (master->exec)
			cleanup_executable(master);
		cleanup_before_exit(master);
		exit(master->exit_status);
	}
}

static void	parent_process_execution(t_master *master, t_token **token)
{
	if (master->exec->pid != 0)
	{
		if (master->exec->first_cmd == 0)
		{
			close(master->exec->old_pipefd[0]);
			close(master->exec->old_pipefd[1]);
		}
		if ((*token)->next && (*token)->next->type == CMD_PIPE)
		{
			master->exec->old_pipefd[0] = master->exec->pipefd[0];
			master->exec->old_pipefd[1] = master->exec->pipefd[1];
			master->exec->first_cmd = false;
			*token = (*token)->next->next;
		}
		else if ((*token)->next)
			*token = (*token)->next->next;
		else
			*token = (*token)->next;
		if (master->exec)
			cleanup_executable(master);
	}
}

static void	handle_execution(t_master *master, pid_t *pids, int *num_pids)
{
	t_cmd_type	type;
	t_token		*token;

	token = master->token;
	while (token)
	{
		type = prepare_execution(master, token);
		if (type == CMD_ERROR)
			break ;
		child_process_execution(master, token, type);
		parent_process_execution(master, &token);
		if (master->exec->pid != 0)
			pids[(*num_pids)++] = master->exec->pid;
	}
}

void	launch_execution(t_master *master)
{
	int		i;
	int		status;
	int		num_pids;
	pid_t	pids[MAX_PIDS];

	status = 0;
	num_pids = 0;
	master->exit_status = 0;
	init_exec(master);
	launch_heredoc(master);
	handle_execution(master, pids, &num_pids);
	if (master->exec->first_cmd == false)
	{
		close(master->exec->old_pipefd[0]);
		close(master->exec->old_pipefd[1]);
	}
	i = -1;
	while (++i < num_pids)
	{
		while ((waitpid(pids[i], &status, 0)) > 0)
			if (WIFEXITED(status) && master->exit_status != 127)
				master->exit_status = WEXITSTATUS(status);
	}
}
