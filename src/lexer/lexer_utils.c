/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:41:29 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/20 10:47:39 by ladloff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "libft.h"

int	is_heredoc_pipe(t_token **token_lst)
{
	t_token			*current;

	current = *token_lst;
	while (current)
	{
		if (current->type == T_PIPE && !current->next)
		{
			free_token_list(*token_lst);
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	start_operator(t_token_type type)
{
	if (type == T_BUILTIN)
		return (EXIT_SUCCESS);
	else if (type == T_PIPE)
		printf(ESTR_UNEXP, '|');
	else if (type != T_BUILTIN)
		printf(ESTR_OPSTART);
	return (EXIT_FAILURE);
}

int	is_clean(t_token **token_lst)
{
	t_token			*current;
	char			type;
	const char		*ops[OP] = {"|", "<", "<<", ">", ">>"};

	current = *token_lst;
	while (current && current->next)
	{
		if (current->next->type == T_BUILTIN && !ft_strlen(current->next->data)
			&& current->type > T_BUILTIN)
		{
			type = *ops[current->type - 1];
			printf(ESTR_UNEXP, type);
			free_token_list(*token_lst);
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
			if ((line_read[i - 1] == '\\' && line_read[i - 2] == '\\')
				|| (line_read[i - 1] != '\\'))
				if (!in_double_quote)
					in_single_quote = !in_single_quote;
		}
		else if (line_read[i] == '\"')
		{
			if ((line_read[i - 1] == '\\' && line_read[i - 2] == '\\')
				|| (line_read[i - 1] != '\\'))
				if (!in_single_quote)
					in_double_quote = !in_double_quote;
		}
	}
	return (return_value(in_single_quote, in_double_quote));
}

// void	print_token_list(t_token *token_list)
// {
// 	size_t	i;
// 	t_token	*current;

// 	i = 0;
// 	current = token_list;
// 	printf("\n\n---------------TABLEAU DE TOKENS-------------\n");
// 	printf("| %-5s | %-25s | %-5s |\n", "#", "Token Data", "Type");
// 	printf("---------------------------------------------\n");
// 	while (current)
// 	{
// 		printf("| %-5zu | %-25s | %-5d |\n", i, current->data, current->type);
// 		current = current->next;
// 		i++;
// 	}
// 	printf("---------------------------------------------\n\n\n");
// }
