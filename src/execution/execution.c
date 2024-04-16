/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:20:24 by ladloff           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/04/16 15:10:47 by alfloren         ###   ########.fr       */
=======
/*   Updated: 2024/04/16 15:09:18 by ladloff          ###   ########.fr       */
>>>>>>> f37a0b4a4645d21f9b51ee1b806d70a4a12e2133
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

static void	prepare_execution(t_master *master, t_token *token)
{
	t_cmd_type	type;

	if (token->type != CMD_NOCMD)
	{
		create_arguments(master, token);
		replace_argv_without_quotes(master);
		update_executable_path(master, master->env);
		type = identify_builtin_command(master->argv[0]);
	}
	else
		type = CMD_NOCMD;
	if (!token->next && master->exec->pipefd[0] == -1
		&& (type >= CMD_CD && type <= CMD_EXPORT))
		master->exit_status = execute_builtin(master, type);
	else
	{
		if (token->next && token->next->type == CMD_PIPE)
			if (pipe(master->exec->pipefd) == -1)
				error_exit(master, "pipe (execute_pipeline)");
		master->exec->pid = fork();
		if (master->exec->pid == -1)
			error_exit(master, "fork (execute_pipeline)");
	}
	master->exec->type = type;
}

static void	child_process(t_master *master, t_token *token, t_cmd_type type)
{
	if (master->exec->pid == 0)
	{
		if (master->exec->first_cmd == false)
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
		if (type == CMD_NOCMD)
			no_command(master, &token->redir);
		else
			execution(master, token, type);
		cleanup_before_exit(master);
		exit(master->exit_status);
	}
}

static void	parent_process(t_master *master, t_token **token, bool no_cmd)
{
	if (master->exec->first_cmd == false)
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
	if (!no_cmd && !ft_strncmp(master->argv[0], master->program_name,
			ft_strlen(master->program_name)))
		block_signals();
	else
		set_sigaction_exec(master);
	free_string_array(&master->argv);
}

static int	handle_execution(t_master *master, int *num_pids)
{
	t_token		*token;
	bool		no_cmd;

	no_cmd = false;
	token = master->token;
	while (token)
	{
		if (token->type == CMD_NOCMD)
			no_cmd = true;
		prepare_execution(master, token);
		child_process(master, token, master->exec->type);
		parent_process(master, &token, no_cmd);
		master->pid_list[(*num_pids)++] = master->exec->pid;
	}
	return (0);
}

void	launch_execution(t_master *master)
{
	int	i;
	int	status;
	int	num_pids;

	status = 0;
	num_pids = 0;
	master->exit_status = 0;
	init_exec(master);
	launch_heredoc(master);
	if (master->last_command_exit_value == 256)
		return ;
	if (handle_execution(master, &num_pids) == 1)
		return ;
	i = -1;
	while (++i < num_pids)
		while ((waitpid(master->pid_list[i], &status, 0)) > 0)
			if (WIFEXITED(status))
				master->exit_status = WEXITSTATUS(status);
	if (master->exec->first_cmd == false)
	{
		close(master->exec->old_pipefd[0]);
		close(master->exec->old_pipefd[1]);
	}
}
