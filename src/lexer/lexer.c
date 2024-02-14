/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/14 12:56:43 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

void	create_token_node(t_master *master, t_cmd_type type, char *data,
	t_token **token)
{
	t_token	*new_node;

	new_node = ft_calloc(1, sizeof(t_token));
	if (!new_node)
	{
		free(data);
		ft_error_exit(master, "ft_calloc (create_token_node)", ENOMEM, false);
		return ;
	}
	new_node->data = data;
	new_node->type = type;
	if (*token == NULL)
	{
		*token = new_node;
		(*token)->last = new_node;
	}
	else
	{
		(*token)->last->next = new_node;
		(*token)->last = new_node;
	}
}

int	manage_redirection(const char *line_read, size_t *i, bool redir)
{
	if (is_in_quotes(line_read, i))
		return (EXIT_SUCCESS);
	if (line_read[*i] == '\0')
	{
		if (redir)
			ft_dprintf(STDERR_FILENO, ESTR_OPSTART_P1 ESTR_OPSTART_P2);
		return (EXIT_FAILURE);
	}
	else if (line_read[*i] == '>' || line_read[*i] == '<')
	{
		if (redir)
			ft_dprintf(STDERR_FILENO, ESTR_UNEXP, line_read[*i]);
		return (EXIT_FAILURE);
	}
	while (!ft_isspace(line_read[*i])
		&& line_read[*i] != '|'
		&& line_read[*i] != '<'
		&& line_read[*i] != '>'
		&& line_read[*i])
		(*i)++;
	return (EXIT_SUCCESS);
}

int	create_node_command(t_master *master, char *line_read,
			t_token **token, size_t *j)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*data;

	i = (*j);
	start = 0;
	end = 0;
	data = NULL;
	while (line_read[i] && line_read[i] != '|')
	{
		next_sign(line_read, &i, &start, &end);
		if (start != end)
			if (create_data_command(line_read, (size_t [2]){start, end},
				&data))
				return (EXIT_FAILURE);
		pass_redirection(line_read, &i);
	}
	if (data && *data)
		create_token_node(master, CMD_OTHERS, data, token);
	return (EXIT_SUCCESS);
}

int	create_nodes_redir(t_master *master, char *line_read,
		t_token **token, size_t *i)
{
	size_t		k;

	while (line_read[(*i)] && line_read[(*i)] != '|')
	{
		next_sign_redir(line_read, i, &k);
		if (line_read[(*i)] == '|' || line_read[(*i)] == '\0')
		{
			if (line_read[(*i)] == '|')
			{
				create_token_node(master, CMD_PIPE, NULL, token);
				(*i)++;
				(*token)->last->redir = NULL;
			}
			return (EXIT_SUCCESS);
		}
		else if (is_in_quotes(line_read, i))
			continue ;
		else
			if (create_redir(master, line_read, token,
					(size_t * [2]){i, &k}) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	launch_lexer(t_master *master, char *line_read, t_token **token)
{
	size_t	i;

	i = 0;
	if (!is_matched_quotes(master, line_read))
		return (EXIT_FAILURE);
	while (line_read[i])
	{
		if (create_node_command(master, line_read, token, &i)
			!= EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (create_nodes_redir(master, line_read, token, &i)
			!= EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (exit_handler(master, token));
}
