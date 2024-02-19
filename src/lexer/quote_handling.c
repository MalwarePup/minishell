/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:36:01 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/18 14:34:43 by ladloff          ###   ########.fr       */
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
bool	is_escaped(const char *str, int index)
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

bool	is_matched_quotes(t_master *master, const char *str)
{
	size_t	i;
	char	current_quote;

	i = -1;
	current_quote = '\0';
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (!is_escaped(str, i) && current_quote == '\0')
				current_quote = str[i];
			else if (!is_escaped(str, i) && current_quote == str[i])
				current_quote = '\0';
		}
	}
	if (current_quote != '\0')
	{
		if (current_quote == '\'')
			ft_dprintf(STDERR_FILENO, ESTR_QUOTE, current_quote);
		else if (current_quote == '\"')
			ft_dprintf(STDERR_FILENO, ESTR_QUOTE, current_quote);
		master->exit_status = EXIT_MISUSE;
		return (false);
	}
	return (true);
}
