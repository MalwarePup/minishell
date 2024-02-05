/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:48:59 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/05 16:10:53 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	creation_pipe(t_exec *exec,
	t_master *master, t_token *token)
{
	if (token->next && token->next->type == CMD_PIPE)
		if (pipe(exec->pipefd) == -1)
			error_exit(master, "pipe (execute_pipeline)");
	exec->pid = fork();
	if (exec->pid == -1)
		error_exit(master, "fork (execute_pipeline)");
}

t_cmd_type	preparation_args(t_master *master, t_token *token)
{
	t_cmd_type	type;

	master->exec = create_arguments(master, token);
	launch_expansion(master, master->exec);
	update_executable_path(master->exec, master->env_list);
	type = execute_command_or_builtin(master, master->exec);
	return (type);
}

void	launch_builtin(t_master *master, t_exec *exec, t_cmd_type type,
	t_token *token)
{
	launch_redirection(master, token->redir, exec);
	g_exit_status = execute_builtin(master, exec, type);
	dup2(exec->stdin_fd, STDIN_FILENO);
	dup2(exec->stdout_fd, STDOUT_FILENO);
}

void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("token->type = %d, token->data = %s\n", token->type, token->data);
		token = token->next;
	}
}