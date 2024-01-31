/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 11:50:40 by alfloren          #+#    #+#             */
/*   Updated: 2024/01/31 10:32:25 by ladloff          ###   ########.fr       */
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
	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
		error_exit(master, "open redirect output");
	if (dup2(fd, STDOUT_FILENO) == -1)
		error_exit(master, "dup2 redirect output");
	close(fd);
}

int	launch_redirection(t_master *master, t_token *tmp, t_exec *exec)
{
	t_token	*token;

	token = tmp;

	while (token && token->next)
	{
		if (token->type == T_RED_IN)
		{
			redirect_input(master, token->next->data, &exec);
			return (EXIT_FAILURE);
		}
		if (token->type == T_RED_OUT)
		{
			redirect_output(master, token->next->data, 0, &exec);
			return (EXIT_FAILURE);
		}
		if (token->type == T_D_RED_OUT)
		{
			redirect_output(master, token->next->data, 1, &exec);
			return (EXIT_FAILURE);
		}
		token = token->next;
	}
	exec->redir = false;
	return (EXIT_SUCCESS);
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
