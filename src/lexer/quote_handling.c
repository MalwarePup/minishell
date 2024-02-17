/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:36:01 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/17 12:32:22 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

/**
 * Checks if a character in a string is escaped.
 *
 * @param str The string to check.
 * @param index The index of the character to check.
 * @return 1 if the character is escaped, 0 otherwise.
 */
static bool	is_escaped(const char *str, int index)
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

bool	is_matched_quotes(t_master *master, const char *line_read)
{
	size_t	i;
	char	current_quote;

	i = -1;
	current_quote = '\0';
	while (line_read[++i])
	{
		if (line_read[i] == '\'' || line_read[i] == '\"')
		{
			if (!is_escaped(line_read, i) && current_quote == '\0')
				current_quote = line_read[i];
			else if (!is_escaped(line_read, i) && current_quote == line_read[i])
				current_quote = '\0';
		}
	}
	if (current_quote != '\0')
	{
		if (current_quote == '\'')
			ft_dprintf(STDERR_FILENO, ESTR_QUOTE, current_quote);
		else if (current_quote == '\"')
			ft_dprintf(STDERR_FILENO, ESTR_QUOTE, current_quote);
		master->exit_status = 2;
		return (false);
	}
	return (true);
}
