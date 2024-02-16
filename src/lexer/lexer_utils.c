/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:41:29 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 14:37:21 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "ft_dprintf.h"
#include "libft.h"

int	is_heredoc_pipe(t_token **token)
{
	t_token	*current;

	current = *token;
	while (current)
	{
		if (current->type == CMD_PIPE && !current->next)
		{
			free_token(token);
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	start_operator(t_master *master, t_cmd_type type)
{
	if (type == CMD_OTHERS)
		return (EXIT_SUCCESS);
	else if (type == CMD_PIPE)
	{
		master->exit_status = 2;
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '|');
	}
	else if (type != CMD_OTHERS)
	{
		master->exit_status = 2;
		ft_dprintf(STDERR_FILENO, ESTR_OPSTART_P1 ESTR_OPSTART_P2);
	}
	return (EXIT_FAILURE);
}

int	is_clean(t_token **token)
{
	char		type;
	t_token		*current;
	const char	*ops[OP] = {"|", "<", "<<", ">", ">>"};

	current = *token;
	while (current && current->next)
	{
		if (current->next->type == CMD_OTHERS && !ft_strlen(current->next->data)
			&& current->type > CMD_OTHERS)
		{
			type = *ops[current->type - 1];
			printf(ESTR_UNEXP, type);
			free_token(token);
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
