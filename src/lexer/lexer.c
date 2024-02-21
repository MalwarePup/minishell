/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/21 12:20:26 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

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

static int	creates_redir(t_master *master, t_lexer *lexer, size_t *i)
{
	t_cmd_type	type;

	type = CMD_OTHERS;
	type = redir_type(master->line_read, i);
	if (type == CMD_ERROR)
		return (master->exit_status = EXIT_MISUSE,
			clean_lexer(lexer), EXIT_FAILURE);
	lexer->data_redir = creates_data(master, lexer, i, false);
	lexer->data_redir = trim_spaces(master, lexer, lexer->data_redir);
	if (!lexer->data_redir)
		return (exit_redir(master, lexer, *i), EXIT_FAILURE);
	create_token_node(master, lexer, type, false);
	return (EXIT_SUCCESS);
}

void	creates_command(t_master *master, t_lexer *lexer, size_t *i)
{
	char	*data_command;

	data_command = creates_data(master, lexer, i, true);
	if (!data_command)
		return ;
	lexer->data_command = ft_strjoin2(lexer->data_command, data_command);
	if (!(lexer->data_command))
		lexer_exit(master, lexer, "strjoin2 error in creates_command");
}

int	creates_command_pipe(t_master *master, t_lexer *lexer, size_t *i)
{
	while (master->line_read[*i] && master->line_read[*i] != '|')
	{
		while (ft_isspace(master->line_read[(*i)]) && master->line_read[(*i)])
			(*i)++;
		if (master->line_read[(*i)] == '|')
			break ;
		if (master->line_read[(*i)] == '<' || master->line_read[(*i)] == '>')
		{
			if (creates_redir(master, lexer, i) == EXIT_FAILURE)
				return (clean_lexer(lexer), EXIT_FAILURE);
		}
		else
			creates_command(master, lexer, i);
	}
	return (EXIT_SUCCESS);
}

void	create_token(t_master *master, t_lexer *lexer, t_token **token)
{
	t_token	*tmp;

	tmp = NULL;
	if (lexer->data_command)
	{
		create_token_node(master, lexer, CMD_OTHERS, true);
		if (lexer->redirect)
			ft_lstdupp(master, lexer, &lexer->redirect, &tmp);
		(*token)->last->redir = tmp;
	}
}

int	launch_lexer(t_master *master)
{
	size_t	i;
	t_lexer	lexer;

	init_lexer(&lexer);
	i = 0;
	while (master->line_read[i])
	{
		if (master->line_read[i] == '|')
		{
			create_token_node(master, &lexer, CMD_PIPE, true);
			if (start_operator(master) || two_consecutive_pipe(master))
				return (clean_lexer(&lexer), EXIT_FAILURE);
			i++;
			continue ;
		}
		if (creates_command_pipe(master, &lexer, &i) == EXIT_FAILURE)
			return (clean_lexer(&lexer), EXIT_FAILURE);
		create_token(master, &lexer, &master->token);
		clean_lexer(&lexer);
	}
	return (last_operator(master));
}
