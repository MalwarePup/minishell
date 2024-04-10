/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 10:51:38 by macbookpro        #+#    #+#             */
/*   Updated: 2024/04/10 08:50:59 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

int	handle_redir(t_master *master, t_token *token, bool *is_input,
		bool *is_output)
{
	int	exit_status;

	exit_status = 2;
	replace_redir(master, &token->data);
	if (token->type == CMD_RED_IN || token->type == CMD_D_RED_IN)
	{
		if (token->type == CMD_RED_IN)
			exit_status = redirect_cmd(master, token->data, O_RDONLY,
					STDIN_FILENO);
		else if (token->type == CMD_D_RED_IN)
			exit_status = redirect_cmd(master, token->data, O_RDONLY,
					STDIN_FILENO);
		if (exit_status == true)
			*is_input = true;
	}
	else if (token->type == CMD_RED_OUT || token->type == CMD_D_RED_OUT)
	{
		if (token->type == CMD_RED_OUT)
			exit_status = redirect_cmd(master, token->data,
					O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
		else if (token->type == CMD_D_RED_OUT)
			exit_status = redirect_cmd(master, token->data,
					O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
		if (exit_status == true)
			*is_output = true;
	}
	if (exit_status == false)
		return (1);
	return (0);
}
