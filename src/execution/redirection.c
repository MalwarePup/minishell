/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 11:50:40 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/12 19:03:47 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

static void	redirect(t_master *master, char *file, int flag, int fd)
{
	int	new_fd;

	new_fd = open(file, flag, 0644);
	if (new_fd == -1)
		error_exit(master, file, true);
	if (dup2(new_fd, fd) == -1)
		error_exit(master, file, true);
	close(new_fd);
}

void	launch_redirection(t_master *master, t_token *token)
{
	while (token)
	{
		if (token->type == CMD_RED_IN)
			redirect(master, token->data, O_RDONLY, STDIN_FILENO);
		else if (token->type == CMD_RED_OUT)
			redirect(master, token->data,
				O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
		else if (token->type == CMD_D_RED_OUT)
			redirect(master, token->data,
				O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
		else if (token->type == CMD_D_RED_IN)
			redirect(master, token->data, O_RDONLY, STDIN_FILENO);
		token = token->next;
	}
}
