/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/15 20:22:05 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

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
		if (is_not_escaped(line_read, i)
			&& (((line_read[i] == '\''
						|| line_read[i] == '\"')
					&& *quote == 0)
				|| (line_read[i] == *quote && *quote != 0)))
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

int	creates_redir(char *line_read, size_t *i,
	t_token **redirect)
{
	t_cmd_type	type;
	char		*redir;
	char		sign;

	type = CMD_OTHERS;
	sign = line_read[*i];
	type = redir_type(line_read, i);
	if (type == CMD_ERROR)
		return (EXIT_FAILURE);
	redir = creates_data(line_read, i, false);
	if (!redir)
		return (EXIT_FAILURE);
	redir = trim_spaces(redir);
	if (!redir)
		return (EXIT_FAILURE);
	if (replace_redir_without_quotes(&redir) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (create_token_node(type, &redir, redirect) == EXIT_FAILURE)
		return (free(redir), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*creates_data(char *line_read,
				size_t *i, bool command)
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
		data = calloc((end - (*i) + 1), sizeof(char));
		if (!data)
			return (NULL);
		ft_strlcpy(data, &line_read[(*i)], end - (*i) + 1);
		*i = end;
	}
	return (data);
}
