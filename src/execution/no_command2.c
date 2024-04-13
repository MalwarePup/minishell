/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_command2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 14:13:33 by macbookpro        #+#    #+#             */
/*   Updated: 2024/04/13 14:23:09 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

void	redirect_initial(t_master *master, t_redir *redir)
{
	if (redir->is_input == true)
		if (dup2(redir->stdin, STDIN_FILENO) == -1)
			error_exit(master, "dup2: stdin");
	if (redir->is_input == false)
		if (dup2(redir->stdout, STDOUT_FILENO) == -1)
			error_exit(master, "dup2: stdout");
}

bool	redirect_cmd(t_master *master, char *file, t_redir *redir)
{
	int	new_fd;

	new_fd = open(file, redir->flag, 0644);
	if (new_fd == -1)
	{
		perror(file);
		master->exit_status = 1;
		redirect_initial(master, redir);
		free(redir);
		return (false);
	}
	if (dup2(new_fd, redir->fd) == -1)
		error_exit(master, file);
	close(new_fd);
	return (true);
}

int	handle_input_redir(t_master *master, t_token *token, t_redir *redir)
{
	int	exit_status;

	redir->flag = O_RDONLY;
	redir->fd = STDIN_FILENO;
	exit_status = redirect_cmd(master, token->data, redir);
	if (exit_status == true)
		redir->is_input = true;
	return (exit_status);
}

int	handle_output_redir(t_master *master,
	t_token *token, t_redir *redir)
{
	int	exit_status;

	redir->fd = STDOUT_FILENO;
	if (token->type == CMD_RED_OUT)
		redir->flag = O_WRONLY | O_CREAT | O_TRUNC;
	else
		redir->flag = O_WRONLY | O_CREAT | O_APPEND;
	exit_status = redirect_cmd(master, token->data, redir);
	if (exit_status == true)
		redir->is_output = true;
	return (exit_status);
}
