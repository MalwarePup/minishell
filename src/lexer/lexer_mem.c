/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:13:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 19:13:53 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"

void	free_token_list(t_token *token_list)
{
	t_token	*next;
	t_token	*current;

	current = token_list;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
}

void	create_token_node(t_master *master, t_token_type type, char *data,
	t_token **token_list)
{
	t_token	*new_node;

	new_node = ft_calloc(1, sizeof(t_token));
	if (!new_node)
	{
		free(data);
		ft_error_exit(master, "ft_calloc (create_token_node)", ENOMEM);
		return ;
	}
	new_node->data = data;
	new_node->type = type;
	if (*token_list == NULL)
	{
		*token_list = new_node;
		(*token_list)->last = new_node;
	}
	else
	{
		(*token_list)->last->next = new_node;
		(*token_list)->last = new_node;
	}
}
