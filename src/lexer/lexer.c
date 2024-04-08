/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/08 16:17:29 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

static int	creates_redir(t_master *master, t_lexer *lexer, size_t *i)
{
	t_cmd_type	type;

	type = CMD_OTHERS;
	type = redir_type(master->line_read, i);
	if (type == CMD_ERROR)
	{
		clean_lexer(lexer);
		master->exit_status = EXIT_MISUSE;
		return (1);
	}
	lexer->data_redir = creates_data(master, lexer, i, false);
	if (!lexer->data_redir)
	{
		exit_redir(master, lexer, *i);
		return (1);
	}
	create_token_node(master, lexer, type, false);
	return (EXIT_SUCCESS);
}

static void	creates_command(t_master *master, t_lexer *lexer, size_t *i)
{
	char	*data_command;

	data_command = creates_data(master, lexer, i, true);
	if (!data_command)
		return ;
	lexer->data_command = ft_strjoin2(lexer->data_command, data_command);
	if (!(lexer->data_command))
		lexer_exit(master, lexer, "strjoin2 error in creates_command");
}

static int	creates_command_and_redir(t_master *master, t_lexer *lexer,
		size_t *i)
{
	while (master->line_read[*i] && master->line_read[*i] != '|')
	{
		while (ft_isspace(master->line_read[*i]) && master->line_read[*i])
			(*i)++;
		if (master->line_read[*i] == '|')
			break ;
		if (master->line_read[*i] == '<' || master->line_read[*i] == '>')
		{
			if (creates_redir(master, lexer, i))
				return (1);
		}
		else
			creates_command(master, lexer, i);
	}
	return (EXIT_SUCCESS);
}
void print_token(t_token *token)
{
	t_token *tmp = token;
	while (tmp)
	{
		ft_dprintf(1, "type: %d\n", tmp->type);
		ft_dprintf(1, "data: %s\n", tmp->data);
		print_token(tmp->redir);
		tmp = tmp->next;
	}
}

static void	create_node_with_redir(t_master *master, t_lexer *lexer,
		t_token **token)
{
	if (lexer->data_command)
	{
		create_token_node(master, lexer, CMD_OTHERS, true);
		(*token)->last->redir = lexer->redirect;
		lexer->redirect = NULL;
	}
	else if (lexer->redirect)
	{
		create_token_node(master, lexer, CMD_NOCMD, true);
		(*token)->last->redir = lexer->redirect;
		lexer->redirect = NULL;
	}
}

int	launch_lexer(t_master *master)
{
	size_t	i;
	t_lexer	lexer;

	i = 0;
	ft_memset(&lexer, 0, sizeof(t_lexer));
	while (master->line_read[i])
	{
		if (master->line_read[i] == '|')
		{
			create_token_node(master, &lexer, CMD_PIPE, true);
			if (start_operator(master) || two_consecutive_pipe(master))
				return (clean_lexer(&lexer), 1);
			i++;
			continue ;
		}
		if (creates_command_and_redir(master, &lexer, &i))
			return (1);
		create_node_with_redir(master, &lexer, &master->token);
		clean_lexer(&lexer);
	}
	return (last_operator(master));
}
