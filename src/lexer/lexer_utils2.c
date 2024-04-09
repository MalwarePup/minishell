/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/09 08:37:14 by ladloff          ###   ########.fr       */
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
	if (line_read[*i] == redir)
	{
		if (line_read[*i + 1] != redir)
		{
			if (redir == '>')
				type = CMD_D_RED_OUT;
			else
				type = CMD_D_RED_IN;
			(*i)++;
		}
		else
			return (ft_dprintf(STDERR_FILENO, ESTR_UNEXP, redir), CMD_ERROR);
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

void	exit_redir(t_master *master, t_lexer *lexer, size_t i)
{
	master->exit_status = EXIT_MISUSE;
	if (master->line_read[i++] == '|')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '|');
	else if (master->line_read[i] == 0)
		ft_dprintf(STDERR_FILENO, ESTR_OPSTART_P1 ESTR_OPSTART_P2);
	else if (master->line_read[i] == '<' && master->line_read[i] != '<')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '<');
	else if (master->line_read[i] == '>' && master->line_read[i] != '>')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '>');
	else if (master->line_read[i] == '>' && master->line_read[i] == '>')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP_STR, ">>");
	else if (master->line_read[i] == '<' && master->line_read[i] == '<')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP_STR, "<<");
	else
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, master->line_read[i]);
	clean_lexer(lexer);
	free_token(&master->token);
}

bool	is_valid_character(char c, bool command, char *quote)
{
	if (c && ((c != '|' && (!ft_isspace(c) || command) && c != '<' && c != '>'
				&& *quote == 0) || *quote != 0))
	{
		if (((c == '\'' || c == '"') && *quote == 0) || (c == *quote
					&& *quote != 0))
		{
			if (*quote != 0)
				*quote = 0;
			else
				*quote = c;
		}
		return (true);
	}
	return (false);
}

char	*creates_data(t_master *master, t_lexer *lexer, size_t *i,
	bool command)
{
	size_t	start;
	size_t	end;
	char	*data;
	char	quote;

	start = *i;
	data = NULL;
	quote = 0;
	while (ft_isspace(master->line_read[start]) && master->line_read[start])
		start++;
	end = start;
	while (is_valid_character(master->line_read[end], command, &quote))
		end++;
	if (start != end)
	{
		data = malloc((end - start + 1) * sizeof(char));
		if (!data)
			lexer_exit(master, lexer, "malloc (creates_data)");
		ft_strlcpy(data, &master->line_read[start], end - start + 1);
		*i = end;
	}
	return (data);
}
