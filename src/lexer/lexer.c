/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/19 10:35:55 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

static int	to_pass(char *str, char *quote, char *ex_quote, size_t *i)
{
	*ex_quote = *quote;
	condition_while(str, *i, true, quote);
	if (*ex_quote != *quote)
	{
		(*i)++;
		return (true);
	}
	return (false);
}

// void	replace_argv_without_quotes(t_master *master)
// {
// 	char	*new_str;
// 	char	quote;
// 	char	ex_quote;
// 	size_t	ij[2];
// 	size_t	i;

// 	quote = 0;
// 	ex_quote = 0;
// 	i = 0;
// 	ij[0] = 0;
// 	ij[1] = 0;
// 	while (master->argv[i])
// 	{
// 		new_str = malloc((ft_strlen(master->argv[i]) + 1) * sizeof(char));
// 		if (!new_str)
// 			error_exit(master, "malloc (replace_argv_without_quotes)");
// 		while (master->argv[i][ij[0]])
// 		{
// 			if (to_pass(master->argv[i], &quote, &ex_quote, &ij[0]))
// 				continue ;
// 			new_str[ij[1]++] = master->argv[i][ij[0]++];
// 		}
// 		new_str[ij[1]] = '\0';
// 		free(master->argv[i]);
// 		master->argv[i] = new_str;
// 		i++;
// 	}
// }

static int	replace_redir_without_quotes(char **str)
{
	char	*new_str;
	char	*test;
	char	quote;
	char	ex_quote;
	size_t	ij[2];

	quote = 0;
	ex_quote = 0;
	ij[0] = 0;
	ij[1] = 0;
	test = malloc(sizeof(char) * (ft_strlen(*str) + 1));
	ft_strlcpy(test, *str, ft_strlen(*str) + 1);
	new_str = malloc(sizeof(char)
			* (ft_strlen(*str) + 1));
	if (!new_str)
		return (free(*str), EXIT_FAILURE);
	while (test[ij[0]])
	{
		if (to_pass(test, &quote, &ex_quote, &ij[0]))
			continue ;
		new_str[ij[1]++] = test[ij[0]++];
	}
	new_str[ij[1]] = '\0';
	return (free(*str), *str = new_str, free(test), EXIT_SUCCESS);
}


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
		return (free_token(redirect), free(redir),
			master->exit_status = EXIT_MISUSE,
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
				return (free(*data), master->exit_status = EXIT_MISUSE,
					EXIT_FAILURE);
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
		if (creates_command_pipe(master, &i, &data, &redirect) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (create_token(&data, token, &redirect) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (exit_handler(master, token));
}
