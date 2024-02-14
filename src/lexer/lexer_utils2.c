/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/14 12:30:57 by ladloff          ###   ########.fr       */
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

int	exit_handler(t_master *master, t_token **token)
{
	int	i;

	i = EXIT_FAILURE;
	if (!(*token))
		return (i);
	if ((*token)->type == CMD_OTHERS)
		i = EXIT_SUCCESS;
	else if ((*token)->type == CMD_PIPE)
		printf(ESTR_UNEXP, '|');
	else if ((*token)->type == CMD_RED_IN
		|| (*token)->type == CMD_RED_OUT
		|| (*token)->type == CMD_D_RED_IN
		|| (*token)->type == CMD_D_RED_OUT)
		i = EXIT_SUCCESS;
	else if ((*token)->type != CMD_OTHERS)
		printf(ESTR_OPSTART);
	if (i || is_clean(token) || is_heredoc_pipe(token))
	{
		master->exit_status = 2;
		return (free_token(token), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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
