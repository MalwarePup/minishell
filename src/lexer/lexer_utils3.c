/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:51:40 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/14 12:41:17 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static char	*trim_spaces1(const char *str, size_t startend[2])
{
	size_t		i;
	size_t		length;
	char		*trimmed_str;
	const char	ops[4] = "|&<>";

	if (startend[0] >= ft_strlen(str) || startend[1] >= ft_strlen(str))
		return (NULL);
	i = 0;
	while (i < 4 && ops[i])
	{
		if (ops[i] == str[startend[1]])
			startend[1]--;
		i++;
	}
	while (ft_isspace(str[startend[0]]) && startend[0] <= startend[1])
		startend[0]++;
	while (ft_isspace(str[startend[1]]) && startend[1] >= startend[0])
		startend[1]--;
	length = startend[1] - startend[0] + 1;
	trimmed_str = malloc((length + 1) * sizeof(char));
	if (!trimmed_str)
		return (NULL);
	ft_strlcpy(trimmed_str, &str[startend[0]], length + 1);
	return (trimmed_str);
}

void	next_sign(char *line_read, size_t *i, size_t *start, size_t *end)
{
	while (ft_isspace(line_read[*i]) && line_read[*i])
		(*i)++;
	*start = *i;
	if (!is_in_quotes(line_read, i))
	{
		while (!ft_isspace(line_read[*i])
			&& line_read[*i] != '|'
			&& line_read[*i] != '<'
			&& line_read[*i] != '>'
			&& line_read[*i])
			(*i)++;
	}
	*end = *i;
}

int	create_data_command(char *line_read, size_t startend[2],
	char **tmp)
{
	char	*data;

	if (*tmp && **tmp)
	{
		*tmp = ft_strjoin1(*tmp, " ");
		if (!*tmp)
			return (EXIT_FAILURE);
	}
	else
	{
		*tmp = malloc(sizeof(char));
		if (!*tmp)
			return (EXIT_FAILURE);
		*tmp[0] = '\0';
	}
	data = trim_spaces1(line_read, (size_t[2]){startend[0], startend[1] - 1});
	if (!data)
		return (EXIT_FAILURE);
	*tmp = ft_strjoin2(*tmp, data);
	if (!*tmp)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	pass_redirection(const char *line_read, size_t *i)
{
	if (line_read[*i] == '<' || line_read[*i] == '>')
	{
		if (line_read[*i + 1] == line_read[*i])
			(*i)++;
		(*i)++;
		manage_redirection(line_read, i, false);
	}
}
