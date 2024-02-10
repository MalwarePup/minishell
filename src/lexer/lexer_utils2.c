/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/10 13:28:46 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "libft.h"

int	is_escaped(const char *str, int index)
{
	int	backslashes;

	backslashes = 0;
	while (index > 0 && str[index - 1] == '\\')
	{
		backslashes++;
		index--;
	}
	return (backslashes % 2 != 0);
}

bool	is_in_quotes(const char *line, size_t *i)
{
	while (ft_isspace(line[*i]))
		(*i)++;
	if (line[*i] != '\"' && line[*i] != '\'')
		return (false);
	if (line[*i] == '\"')
	{
		(*i)++;
		while (line[*i] && line[*i] != '\"')
			(*i)++;
	}
	else
	{
		(*i)++;
		while (line[*i] && line[*i] != '\'')
			(*i)++;
	}
	(*i)++;
	return (true);
}

int	exit_handler(t_token **token_lst)
{
	int	i;

	i = EXIT_FAILURE;
	if (!(*token_lst))
		return (i);
	if ((*token_lst)->type == CMD_OTHERS)
		i = EXIT_SUCCESS;
	else if ((*token_lst)->type == CMD_PIPE)
		printf(ESTR_UNEXP, '|');
	else if ((*token_lst)->type == CMD_RED_IN
		|| (*token_lst)->type == CMD_RED_OUT
		|| (*token_lst)->type == CMD_D_RED_IN
		|| (*token_lst)->type == CMD_D_RED_OUT)
		i = EXIT_SUCCESS;
	else if ((*token_lst)->type != CMD_OTHERS)
		printf(ESTR_OPSTART);
	if (i || is_clean(token_lst) || is_heredoc_pipe(token_lst))
	{
		g_exit_status = 2;
		return (free_token_list(token_lst), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

t_cmd_type	isnot_builtins(char c, const char *line_read, size_t *i)
{
	size_t					j;
	const char				*ops[OP] = {"|", "<", "<<", ">", ">>"};
	const t_cmd_type		types[OP] = {CMD_PIPE, CMD_RED_IN, CMD_D_RED_IN,
		CMD_RED_OUT, CMD_D_RED_OUT};
	t_cmd_type				token_type;

	j = 0;
	token_type = CMD_OTHERS;
	while (j < OP)
	{
		if (c == *ops[j])
		{
			token_type = types[j];
			if (line_read[*i + 1] == c)
			{
				token_type = types[j + 1];
				(*i)++;
			}
			(*i)++;
			break ;
		}
		j++;
	}
	return (token_type);
}

t_cmd_type	is_builtin(const char *line_read, size_t *i)
{
	while (line_read[*i] != '\0'
		&& line_read[*i] != '|'
		&& line_read[*i] != '<'
		&& line_read[*i] != '>')
		(*i)++;
	return (CMD_OTHERS);
}
