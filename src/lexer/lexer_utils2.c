/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/19 12:00:18 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include "ft_dprintf.h"

t_cmd_type	redir_type(char *line_read, size_t *i)
{
	char		redir;
	t_cmd_type	type;

	redir = line_read[(*i)++];
	if (line_read[(*i)] == redir)
	{
		if (line_read[(*i) + 1] != redir)
		{
			if (redir == '>')
				type = CMD_D_RED_OUT;
			else
				type = CMD_D_RED_IN;
			(*i)++;
		}
		else
			return (ft_dprintf(STDOUT_FILENO, ESTR_UNEXP, redir), CMD_ERROR);
	}
	else
	{
		if (redir == '>')
			type = CMD_RED_OUT;
		else
			type = CMD_RED_IN;
	}
	return (type);
}

int	ft_lstdupp(t_token **token, t_token **new)
{
	t_token	*current;
	t_token	*tmp;

	current = *token;
	while (current)
	{
		tmp = ft_calloc(1, sizeof(t_token));
		if (!tmp)
			return (EXIT_FAILURE);
		tmp->data = ft_strdup(current->data);
		if (!tmp->data)
			return (free(tmp), free(tmp->data), free_token(new), EXIT_FAILURE);
		tmp->type = current->type;
		if (*new == NULL)
			*new = tmp;
		else
			(*new)->last->next = tmp;
		(*new)->last = tmp;
		current = current->next;
	}
	return (free_token(token), (*token) = NULL, EXIT_SUCCESS);
}

bool	condition_while(char *line_read, size_t i,
					bool command, char *quote)
{
	if (line_read[i] && ((line_read[i] != '|'
				&& (!ft_isspace(line_read[i])
					|| command) && line_read[i] != '<'
				&& line_read[i] != '>'
				&& *quote == 0) || *quote != 0))
	{
		if (((line_read[i] == '\'' || line_read[i] == '"') && *quote == 0)
			|| (line_read[i] == *quote && *quote != 0))
		{
			if (*quote != 0)
				*quote = 0;
			else
				*quote = line_read[i];
		}
		return (true);
	}
	return (false);
}

char	*creates_data(char *line_read, size_t *i,
	bool command)
{
	size_t	start;
	size_t	end;
	char	*data;
	char	quote;

	start = *i;
	data = NULL;
	quote = 0;
	while (ft_isspace(line_read[start]) && line_read[start])
		start++;
	end = start;
	while (condition_while(line_read, end, command, &quote))
		end++;
	if ((*i) != end)
	{
		data = ft_calloc((end - (*i) + 1), sizeof(char));
		if (!data)
			return (NULL);
		ft_strlcpy(data, &line_read[(*i)], end - (*i) + 1);
		*i = end;
	}
	else
		ft_dprintf(2, ESTR_OPSTART_P1 ESTR_OPSTART_P2);
	return (data);
}
