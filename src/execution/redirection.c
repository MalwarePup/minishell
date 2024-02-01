/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 11:50:40 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/01 16:31:45 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "minishell.h"

static void	redirect_input(t_master *master, char *file, t_exec **exec)
{
	int	fd;

	fd = open(file, O_RDONLY);
	(*exec)->redir = true;
	if (fd == -1)
		error_exit(master, "open redirect input");
	if (dup2(fd, STDIN_FILENO) == -1)
		error_exit(master, "dup2 redirect input");
	close(fd);
}

static void	redirect_output(t_master *master, char *file,
	int append, t_exec **exec)
{
	int	flags;
	int	fd;

	(*exec)->redir = true;
	flags = O_WRONLY | O_CREAT | append;
	fd = open(file, flags, 0644);
	if (fd == -1)
		error_exit(master, "open redirect output");
	if (dup2(fd, STDOUT_FILENO) == -1)
		error_exit(master, "dup2 redirect output");
	close(fd);
}

void	restore_fd(int saved_stdin, int saved_stdout)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		error_exit(NULL, "dup2 restore stdin");
	close(saved_stdin);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		error_exit(NULL, "dup2 restore stdout");
	close(saved_stdout);
}

int	launch_redirection(t_master *master, t_token *token, t_exec *exec)
{
	// printf("%p-====%d\n", *token, (*token)->type);
	// while (token && ((*token)->type == CMD_RED_IN
	// 		|| (*token)->type == CMD_RED_OUT))
	// printf("%p-====%d\n", *token, (*token)->type);
	while (token && token->type != CMD_PIPE)
	{
		if (token->type == CMD_RED_IN)
			redirect_input(master, token->data, &exec);
		if (token->type == CMD_RED_OUT)
			redirect_output(master, token->data, O_TRUNC, &exec);
		if (token->type == CMD_D_RED_OUT)
			redirect_output(master, token->data, O_APPEND, &exec);
		token = token->next;
	}
	// restore_fd(exec->stdin_fd, exec->stdout_fd);
	exec->redir = false;
	return (EXIT_SUCCESS);
}
