/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:20:24 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 10:31:44 by alfloren         ###   ########.fr       */
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

	create_arguments(master, token);
	launch_expansion(master);
	update_executable_path(master->exec, master->env_list);
	type = execute_command_or_builtin(master);
	if (type == CMD_ERROR || (!token->next && (type == CMD_CD
				|| type == CMD_EXPORT)))
	{
		if (!token->next && (type == CMD_CD || type == CMD_EXPORT))
			master->exit_status = execute_builtin(master, type);
		return (CMD_ERROR);
	}
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

static void	child_process(t_master *master, t_token *token, t_cmd_type type)
{
	if (master->exec->pid == 0)
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
		if (type == CMD_OTHERS)
			execute_command(master);
		else
			master->prev_exit_status = execute_builtin(master, type);
		cleanup_executable(master);
		cleanup_before_exit(master);
		exit(master->exit_status);
	}
}

static void	parent_process(t_master *master, t_token **token)
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
	set_sigaction_temp(master);
	cleanup_executable(master);
}

static void	handle_execution(t_master *master, int *num_pids)
{
	t_cmd_type	type;
	t_token		*token;

	token = master->token;
	while (token)
	{
		type = prepare_execution(master, token);
		if (type == CMD_ERROR)
		{
			if (token->next && token->next->type == CMD_PIPE)
			{
				cleanup_executable(master);
				token = token->next->next;
				continue ;
			}
			cleanup_executable(master);
			break ;
		}
		child_process(master, token, type);
		parent_process(master, &token);
		master->pids[(*num_pids)++] = master->exec->pid;
	}
}

void	launch_execution(t_master *master)
{
	int		i;
	int		status;
	int		num_pids;

	status = 0;
	num_pids = 0;
	master->exit_status = 0;
	init_exec(master);
	launch_heredoc(master);
	if (master->prev_exit_status == 131)
		return ;
	handle_execution(master, &num_pids);
	i = -1;
	while (++i < num_pids)
	{
		while ((waitpid(master->pids[i], &status, 0)) > 0)
			if (WIFEXITED(status) && master->exit_status != EXIT_NOT_FOUND)
				master->exit_status = WEXITSTATUS(status);
	}
	if (master->exec->first_cmd == false && master->exec->pipe == true)
	{
		close(master->exec->old_pipefd[0]);
		close(master->exec->old_pipefd[1]);
	}
}
