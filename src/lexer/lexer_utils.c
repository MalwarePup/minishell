/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:41:29 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/21 12:21:30 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "ft_dprintf.h"
#include "libft.h"

int	start_operator(t_master *master)
{
	if (master->token->type == CMD_PIPE)
	{
		master->exit_status = EXIT_MISUSE;
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '|');
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	is_clean(t_token **token)
{
	char		type;
	t_token		*current;
	const char	*ops[] = {"|", "<", "<<", ">", ">>"};

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

int	to_pass(char *str, char *quote, char *ex_quote, size_t *i)
{
	*ex_quote = *quote;
	condition_while(str, *i, true, quote);
	if (*ex_quote != *quote)
	{
		(*i)++;
		return (true);
	}
	return (false);
}
