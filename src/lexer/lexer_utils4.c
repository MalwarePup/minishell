/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:34:53 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/14 12:50:32 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

void	next_sign_redir(char *line_read, size_t *i, size_t	*k)
{
	while (ft_isspace(line_read[(*i)])
		|| (line_read[(*i)] != '|'
			&& line_read[(*i)] != '<'
			&& line_read[(*i)] != '>'
			&& line_read[(*i)] != '\''
			&& line_read[(*i)] != '\"'
			&& line_read[(*i)]))
		(*i)++;
	(*k) = (*i);
}

static t_cmd_type	if_redir(char *line_read, size_t *i, t_cmd_type type)
{
	if (line_read[(*i)] == '<')
	{
		if (line_read[(*i) + 1] != '<')
			type = CMD_RED_IN;
		else
		{
			(*i)++;
			type = CMD_D_RED_IN;
		}
		(*i)++;
	}
	else if (line_read[(*i)] == '>')
	{
		if (line_read[(*i) + 1] != '>')
			type = CMD_RED_OUT;
		else
		{
			(*i)++;
			type = CMD_D_RED_OUT;
		}
		(*i)++;
	}
	return (type);
}

int	create_redir(t_master *master, char *line_read, t_token **token,
	size_t *ik[2])
{
	char		*data;
	t_cmd_type	type;

	type = CMD_OTHERS;
	if (line_read[(*ik[0])] == '>' || line_read[(*ik[0])] == '<')
		type = if_redir(line_read, ik[0], type);
	if (ik[0] != ik[1])
	{
		*ik[1] = *ik[0];
		if (manage_redirection(line_read, ik[0], true) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		data = trim_spaces(master, line_read, *ik[1], *ik[0] - 1);
		if (!data)
			return (EXIT_FAILURE);
		data = replace_redir_without_quotes(master, data);
		if (!(*token))
			create_token_node(master, type, NULL, token);
		create_token_node(master, type, data, &(*token)->last->redir);
	}
	return (EXIT_SUCCESS);
}
