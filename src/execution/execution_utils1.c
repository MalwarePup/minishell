/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:48:59 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/11 14:24:34 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "minishell.h"

void	creation_pipe(t_master *master, t_token *token)
{
	if (token->next && token->next->type == CMD_PIPE)
		if (pipe(master->exec->pipefd) == -1)
			error_exit(master, "pipe (execute_pipeline)", false);
	master->exec->pid = fork();
	if (master->exec->pid == -1)
		error_exit(master, "fork (execute_pipeline)", false);
}

t_cmd_type	preparation_args(t_master *master, t_token *token)
{
	t_cmd_type	type;

	create_arguments(master, token);
	launch_expansion(master);
	update_executable_path(master->exec, master->env_list);
	type = execute_command_or_builtin(master);
	return (type);
}

void	launch_builtin(t_master *master, t_cmd_type type,
	t_token *token)
{
	launch_redirection(master, token->redir);
	master->exit_status = execute_builtin(master, type);
	dup2(master->exec->stdin_fd, STDIN_FILENO);
	dup2(master->exec->stdout_fd, STDOUT_FILENO);
}

void	print_token(t_token *token)
{
	while (token)
	{
		printf("token->type = %d, token->data = %s\n", token->type, token->data);
		token = token->next;
	}
}
