/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 10:51:38 by macbookpro        #+#    #+#             */
/*   Updated: 2024/04/10 08:58:24 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

static int	handle_input_redir(t_master *master, t_token *token, bool *is_input)
{
	int	exit_status;

	exit_status = redirect_cmd(master, token->data, O_RDONLY, STDIN_FILENO);
	if (exit_status == true)
		*is_input = true;
	return (exit_status);
}

static int	handle_output_redir(t_master *master, t_token *token,
		bool *is_output)
{
	int	exit_status;

	if (token->type == CMD_RED_OUT)
		exit_status = redirect_cmd(master, token->data,
				O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
	else
		exit_status = redirect_cmd(master, token->data,
				O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
	if (exit_status == true)
		*is_output = true;
	return (exit_status);
}

int	handle_redir(t_master *master, t_token *token, bool *is_input,
		bool *is_output)
{
	int	exit_status;

	exit_status = 2;
	replace_redir(master, &token->data);
	if (token->type == CMD_RED_IN || token->type == CMD_D_RED_IN)
		exit_status = handle_input_redir(master, token, is_input);
	else if (token->type == CMD_RED_OUT || token->type == CMD_D_RED_OUT)
		exit_status = handle_output_redir(master, token, is_output);
	if (exit_status == false)
		return (1);
	return (0);
}
