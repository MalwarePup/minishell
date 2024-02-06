/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 11:50:40 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/06 16:44:58 by alfloren         ###   ########.fr       */
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

int	launch_redirection(t_master *master, t_token *token)
{
	while (token)
	{
		if (token->type == CMD_RED_IN)
			redirect_input(master, token->data, &(master->exec));
		if (token->type == CMD_RED_OUT)
			redirect_output(master, token->data, O_TRUNC, &(master->exec));
		if (token->type == CMD_D_RED_OUT)
			redirect_output(master, token->data, O_APPEND, &(master->exec));
		if (token->type == CMD_D_RED_IN)
			redirect_input(master, token->data, &(master->exec));
		token = token->next;
	}
	return (EXIT_SUCCESS);
}
