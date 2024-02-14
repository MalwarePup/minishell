/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:41:29 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/14 12:31:20 by ladloff          ###   ########.fr       */
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

static bool	return_value(bool in_single_quote, bool in_double_quote)
{
	if (in_single_quote)
	{
		printf(ESTR_QUOTE);
		return (EXIT_FAILURE);
	}
	else if (in_double_quote)
	{
		printf(ESTR_DQUOTE);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	is_matched_quotes(const char *line_read)
{
	size_t	i;
	bool	in_single_quote;
	bool	in_double_quote;

	i = -1;
	in_single_quote = false;
	in_double_quote = false;
	while (line_read[++i])
	{
		if (line_read[i] == '\'')
		{
			if (!is_escaped(line_read, i) && !in_double_quote)
				in_single_quote = !in_single_quote;
		}
		else if (line_read[i] == '\"')
		{
			if (!is_escaped(line_read, i) && !in_single_quote)
				in_double_quote = !in_double_quote;
		}
	}
	return (return_value(in_single_quote, in_double_quote));
}
