/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:36:01 by ladloff           #+#    #+#             */
/*   Updated: 2024/11/05 18:56:28 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

bool	is_matched_quotes(t_master *master, const char *str)
{
	size_t	i;
	char	current_quote;

	i = 0;
	current_quote = '\0';
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (current_quote == '\0')
				current_quote = str[i];
			else if (current_quote == str[i])
				current_quote = '\0';
		}
		i++;
	}
	if (current_quote != '\0')
	{
		if (current_quote == '\'')
			ft_dprintf(STDERR_FILENO, ESTR_QUOTE, current_quote);
		else if (current_quote == '"')
			ft_dprintf(STDERR_FILENO, ESTR_QUOTE, current_quote);
		master->exit_status = MISUSE;
		return (false);
	}
	return (true);
}
