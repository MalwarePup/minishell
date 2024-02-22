/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:13:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/22 18:30:44 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

void	create_node(char *data, t_token **new_node,
	t_cmd_type type, t_token **token)
{
	(*new_node)->data = data;
	(*new_node)->type = type;
	if (*token == NULL)
	{
		*token = (*new_node);
		(*token)->last = (*new_node);
	}
	else
	{
		(*token)->last->next = (*new_node);
		(*token)->last = (*new_node);
	}
}

char	**ft_data(t_lexer *lexer, bool command)
{
	if (command)
		return (&lexer->data_command);
	return (&lexer->data_redir);
}

t_token	**ft_token_lst(t_master *master, t_lexer *lexer, bool command)
{
	if (command)
		return (&master->token);
	return (&lexer->redirect);
}

void	create_token_node(t_master *master, t_lexer *lexer,
	t_cmd_type type, bool command)
{
	t_token	*new_node;
	char	*tmp;
	char	**data;
	t_token	**lst;

	data = ft_data(lexer, command);
	lst = ft_token_lst(master, lexer, command);
	*data = trim_spaces(master, lexer, *data);
	if (type != CMD_PIPE && (!*data))
		return ;
	new_node = ft_calloc(1, sizeof(t_token));
	if (!new_node)
		lexer_exit(master, lexer, "calloc error in create_token_node");
	if (type == CMD_PIPE)
		create_node(NULL, &new_node, type, lst);
	else
	{
		tmp = ft_strdup(*data);
		if (!tmp)
			lexer_exit(master, lexer, "strdup error in create_token_node");
		create_node(tmp, &new_node, type, lst);
		free(*data);
		*data = NULL;
	}
}
