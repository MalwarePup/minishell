/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/14 12:42:23 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "libft.h"

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

t_cmd_type	is_builtin(const char *line_read, size_t *i)
{
	while (line_read[*i] != '\0'
		&& line_read[*i] != '|'
		&& line_read[*i] != '<'
		&& line_read[*i] != '>')
		(*i)++;
	return (CMD_OTHERS);
}
