/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:20:24 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/13 11:57:02 by alfloren         ###   ########.fr       */
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
		if (token->data && (type == CMD_ERROR || (!token->next
					&& (type >= CMD_CD && type <= CMD_EXPORT))))
		{
			if (!token->next && (type >= CMD_CD && type <= CMD_EXPORT))
				launch_builtin(master, type, token);
			return (CMD_ERROR);
		}
	}
	else
		type = token->type;
	if (token->next && token->next->type == CMD_PIPE)
	{
		if (pipe(master->exec->pipefd) == -1)
			error_exit(master, "pipe (execute_pipeline)", false);
		master->exec->pipe = true;
	}
	master->exec->pid = fork();
	if (master->exec->pid == -1)
		error_exit(master, "fork (execute_pipeline)", false);
	return (type);
}

static void	child_process_execution(t_master *master, t_token *token,
			t_cmd_type type)
{
	if (type != CMD_ERROR)
	{
		if (master->exec->first_cmd == false && master->exec->pipe == true)
		{
			dup2(master->exec->old_pipefd[0], STDIN_FILENO);
			close(master->exec->old_pipefd[0]);
			close(master->exec->old_pipefd[1]);
		}
		if (token->next && token->next->type == CMD_PIPE
			&& master->exec->pipe == true)
		{
			dup2(master->exec->pipefd[1], STDOUT_FILENO);
			close(master->exec->pipefd[0]);
			close(master->exec->pipefd[1]);
		}
		launch_redirection(master, token->redir);
		if (master->exec->pathname)
			execute_command(master);
		else
			execute_builtin(master, type);
		if (master->exec)
			cleanup_executable(master);
		cleanup_before_exit(master);
		exit(master->exit_status);
	}
}

static void	parent_process_execution(t_master *master, t_token **token)
{
	if (master->exec->first_cmd == false && master->exec->pipe == true)
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

static void	handle_execution(t_master *master, pid_t *pids, int *num_pids)
{
	t_cmd_type	type;
	t_token		*token;

	token = master->token;
	while (token)
	{
		type = prepare_execution(master, token);
		if (master->exec->pid == 0)
			child_process_execution(master, token, type);
		else
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
	if (master->exec->first_cmd == false && master->exec->pipe == true)
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
