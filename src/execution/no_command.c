/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 10:51:38 by macbookpro        #+#    #+#             */
/*   Updated: 2024/04/13 16:37:12 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

static int	handle_redir(t_master *master, t_token *token, t_redir *redir)
{
	int	exit_status;

	exit_status = 2;
	replace_redir_without_quotes(master, &token->data);
	if (token->type == CMD_RED_IN || token->type == CMD_D_RED_IN)
		exit_status = handle_input_redir(master, token, redir);
	else if (token->type == CMD_RED_OUT || token->type == CMD_D_RED_OUT)
		exit_status = handle_output_redir(master, token, redir);
	if (exit_status == false)
		return (1);
	return (0);
}

static int	launch_nocommand(t_master *master, t_token *tmp)
{
	t_token	*token;
	t_redir	*redir;

	token = tmp;
	redir = malloc(sizeof(t_redir));
	if (redir == NULL)
		error_exit(master, "malloc error in launch_nocommand.c");
	redir->is_input = false;
	redir->is_output = false;
	redir->stdin = dup(STDOUT_FILENO);
	redir->stdout = dup(STDIN_FILENO);
	while (token)
	{
		if (handle_redir(master, token, redir))
			return (1);
		token = token->next;
	}
	redirect_initial(master, redir);
	free(redir);
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
