/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 10:51:38 by macbookpro        #+#    #+#             */
/*   Updated: 2024/04/16 15:26:15 by alfloren         ###   ########.fr       */
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
	printf("no_commqnd");
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
	t_redir	redir;

	token = tmp;
	ft_memset(&redir, 0, sizeof(t_redir));
	redir.original_stdout = dup(STDOUT_FILENO);
	redir.original_stdin = dup(STDIN_FILENO);
	while (token)
	{
		if (handle_redir(master, token, &redir))
			return (1);
		token = token->next;
	}
	redirect_initial(master, &redir);
	close(redir.original_stdout);
	close(redir.original_stdin);
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
