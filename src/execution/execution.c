/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:20:24 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/21 18:46:11 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"

static void	execute_command(t_master *master)
{
	char	**envp;

	envp = env_list_to_array(master, master->env_list);
	execve(master->exec->pathname, master->exec->argv, envp);
	free_double_ptr(envp);
	cleanup_executable(master);
	error_exit(master, "execve (execute_command)");
}

// Avoid launching a new process if the command is a builtin, 126, 127
static t_cmd_type	prepare_execution(t_master *master, t_token *token,
	t_exec *exec)
{
	t_cmd_type	type;

	master->exec = create_arguments(master, token);
	launch_expansion(master, master->exec);
	update_executable_path(master->exec, master->env_list);
	type = execute_command_or_builtin(master, master->exec);
	if (!token->next && (type >= CMD_CD && type <= CMD_EXPORT))
	{
		g_exit_status = execute_builtin(master, master->exec, type);
		cleanup_executable(master);
		return (CMD_ERROR);
	}
	else if (g_exit_status == EXIT_NOT_FOUND
		|| g_exit_status == EXIT_CANNOT_EXECUTE || g_exit_status == EXIT_MISUSE)
	{
		cleanup_executable(master);
		return (CMD_ERROR);
	}
	if (token->next && token->next->type == T_PIPE)
		if (pipe(exec->pipefd) == -1)
			error_exit(master, "pipe (execute_pipeline)");
	exec->pid = fork();
	if (exec->pid == -1)
		error_exit(master, "fork (execute_pipeline)");
	return (type);
}

static void	child_process_execution(t_master *master, t_token *token,
	t_exec *exec, t_cmd_type type)
{
	if (g_exit_status != 127 && exec->pid == 0)
	{
		if (!exec->first_cmd)
		{
			dup2(exec->old_pipefd[0], STDIN_FILENO);
			close(exec->old_pipefd[0]);
			close(exec->old_pipefd[1]);
		}
		if (token->next && token->next->type == T_PIPE)
		{
			dup2(exec->pipefd[1], STDOUT_FILENO);
			close(exec->pipefd[0]);
			close(exec->pipefd[1]);
		}
		if (master->exec->pathname)
			execute_command(master);
		else
			execute_builtin(master, master->exec, type);
		cleanup_before_exit(master);
		cleanup_executable(master);
		exit(g_exit_status);
	}
}

static void	parent_process_execution(t_master *master, t_token **token,
	t_exec *exec)
{
	if (exec->pid != 0)
	{
		if (!exec->first_cmd)
		{
			close(exec->old_pipefd[0]);
			close(exec->old_pipefd[1]);
		}
		if ((*token)->next && (*token)->next->type == T_PIPE)
		{
			exec->old_pipefd[0] = exec->pipefd[0];
			exec->old_pipefd[1] = exec->pipefd[1];
			exec->first_cmd = false;
		}
		if ((*token)->next)
			*token = (*token)->next->next;
		else
			*token = (*token)->next;
		cleanup_executable(master);
	}
}

void	launch_execution(t_master *master)
{
	t_exec		exec;
	int			status;
	t_cmd_type	type;
	t_token		*token;

	init(&exec, &status);
	g_exit_status = 0;
	token = master->token_list;
	while (token)
	{
		type = prepare_execution(master, token, &exec);
		if (type == CMD_ERROR)
			break ;
		child_process_execution(master, token, &exec, type);
		parent_process_execution(master, &token, &exec);
	}
	if (!exec.first_cmd)
	{
		close(exec.old_pipefd[0]);
		close(exec.old_pipefd[1]);
	}
	while ((waitpid(exec.pid, &status, 0)) > 0)
		if (WIFEXITED(status) && g_exit_status != 127)
			g_exit_status = WEXITSTATUS(status);
}
