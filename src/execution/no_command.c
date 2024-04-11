/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 10:51:38 by macbookpro        #+#    #+#             */
/*   Updated: 2024/04/11 13:00:04 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

static bool	redirect_cmd(t_master *master, char *file, int flag, int fd)
{
	int	new_fd;

	new_fd = open(file, flag, 0644);
	if (new_fd == -1)
	{
		perror(file);
		master->exit_status = 1;
		return (false);
	}
	if (dup2(new_fd, fd) == -1)
		error_exit(master, file);
	close(new_fd);
	return (true);
}

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

static int	handle_redir(t_master *master, t_token *token, bool *is_input,
		bool *is_output)
{
	int	exit_status;

	exit_status = 2;
	replace_redir_without_quotes(master, &token->data);
	if (token->type == CMD_RED_IN || token->type == CMD_D_RED_IN)
		exit_status = handle_input_redir(master, token, is_input);
	else if (token->type == CMD_RED_OUT || token->type == CMD_D_RED_OUT)
		exit_status = handle_output_redir(master, token, is_output);
	if (exit_status == false)
		return (1);
	return (0);
}

static int	launch_nocommand(t_master *master, t_token *tmp)
{
	t_token	*token;
	bool	is_input;
	bool	is_output;
	int		stdin;
	int		stdout;

	token = tmp;
	is_input = false;
	is_output = false;
	stdin = dup(STDIN_FILENO);
	stdout = dup(STDOUT_FILENO);
	while (token)
	{
		if (handle_redir(master, token, &is_input, &is_output))
			return (1);
		token = token->next;
	}
	if (is_input == true)
		if (dup2(stdin, STDIN_FILENO) == -1)
			error_exit(master, "dup2: stdin");
	if (is_output == true)
		if (dup2(stdout, STDOUT_FILENO) == -1)
			error_exit(master, "dup2: stdout");
	return (0);
}

int	no_command(t_master *master, t_token **token)
{
	if ((*token)->type == CMD_NOCMD)
	{
		if (launch_nocommand(master, (*token)->redir))
			return (1);
		*token = (*token)->next;
		if (*token && (*token)->type == CMD_PIPE)
			*token = (*token)->next;
		return (MISUSE);
	}
	return (0);
}
