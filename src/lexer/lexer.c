/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 14:38:11 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

static int	creates_redir(t_master *master, char *line_read, size_t *i,
	t_token **redirect)
{
	t_cmd_type	type;
	char		*redir;

	type = CMD_OTHERS;
	type = redir_type(line_read, i);
	if (type == CMD_ERROR)
		return (free_token(redirect), EXIT_FAILURE);
	redir = creates_data(line_read, i, false);
	if (!redir)
		return (free_token(redirect), EXIT_FAILURE);
	redir = trim_spaces(redir);
	if (!redir)
		return (free_token(redirect), EXIT_FAILURE);
	if (replace_redir_without_quotes(&redir) == EXIT_FAILURE)
		return (free_token(redirect), EXIT_FAILURE);
	if (!redir)
		return (free_token(redirect), free(redir), master->exit_status = 2,
			ft_dprintf(2, ESTR_OPSTART_P1 ESTR_OPSTART_P2), EXIT_FAILURE);
	if (create_token_node(type, &redir, redirect) == EXIT_FAILURE)
		return (free_token(redirect), free(redir), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	creates_command(size_t *i, char **data, char *line_read)
{
	char	*data_command;

	data_command = creates_data(line_read, i, true);
	if (!data_command)
		return (EXIT_FAILURE);
	*data = ft_strjoin2(*data, data_command);
	if (!(*data))
		return (free(*data), free(data_command), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	creates_command_pipe(t_master *master, size_t *i, char **data,
	t_token **redirect)
{
	while (master->line_read[*i] && master->line_read[*i] != '|')
	{
		while (ft_isspace(master->line_read[(*i)]) && master->line_read[(*i)])
			(*i)++;
		if (master->line_read[(*i)] == '|')
			break ;
		if (master->line_read[(*i)] == '<' || master->line_read[(*i)] == '>')
		{
			if (creates_redir(master, master->line_read, i, redirect)
				== EXIT_FAILURE)
				return (free(*data), master->exit_status = 2, EXIT_FAILURE);
		}
		else if (master->line_read[*i] != '|')
		{
			if (creates_command(i, data, master->line_read) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	create_token(char **data, t_token **token, t_token **redirect)
{
	t_token	*tmp;

	tmp = NULL;
	if (data)
	{
		if (create_token_node(CMD_OTHERS, data, token) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (redirect && *redirect)
		{
			if (ft_lstdupp(redirect, &tmp) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			(*token)->last->redir = tmp;
		}
	}
	return (EXIT_SUCCESS);
}

int	launch_lexer(t_master *master, char *line_read, t_token **token)
{
	size_t	i;
	char	*data;
	t_token	*redirect;

	i = 0;
	data = NULL;
	redirect = NULL;
	while (line_read[i])
	{
		if (line_read[i] == '|')
		{
			if (create_token_node(CMD_PIPE, NULL, token)
				== EXIT_FAILURE || start_operator(master, (*token)->type))
				return (EXIT_FAILURE);
			i++;
			continue ;
		}
		if (creates_command_pipe(master, &i, &data, &redirect)
			== EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (create_token(&data, token, &redirect) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (exit_handler(master, token));
}
