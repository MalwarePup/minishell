/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/28 15:25:23 by ladloff          ###   ########.fr       */
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
	if (master->line_read[i] == '|')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '|');
	else if (master->line_read[i] == 0)
		ft_dprintf(STDERR_FILENO, ESTR_OPSTART_P1 ESTR_OPSTART_P2);
	else if (master->line_read[i] == '<' && master->line_read[i + 1] != '<')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '<');
	else if (master->line_read[i] == '>' && master->line_read[i + 1] != '>')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '>');
	else if (master->line_read[i] == '>' && master->line_read[i + 1] == '>')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP_STR, ">>");
	else if (master->line_read[i] == '<' && master->line_read[i + 1] == '<')
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP_STR, "<<");
	else
		ft_dprintf(STDERR_FILENO, ESTR_UNEXP, master->line_read[i]);
	clean_lexer(lexer);
	free_token(&master->token);
}

void	ft_lstdupp(t_master *master, t_lexer *lexer,
			t_token **token, t_token **new)
{
	t_token	*current;
	t_token	*tmp;

	current = *token;
	while (current)
	{
		tmp = ft_calloc(1, sizeof(t_token));
		if (!tmp)
			lexer_exit(master, lexer, "calloc error in ft_lstdupp");
		tmp->data = ft_strdup(current->data);
		if (!tmp->data)
			return (free(tmp),
				lexer_exit(master, lexer, "strdup error in ft_lstdupp"));
		tmp->type = current->type;
		if (*new == NULL)
			*new = tmp;
		else
			(*new)->last->next = tmp;
		(*new)->last = tmp;
		current = current->next;
	}
	free_token(token);
	(*token) = NULL;
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
	while (condition_while(master->line_read, end, command, &quote))
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
