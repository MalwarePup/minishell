/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:13:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 18:45:34 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"

/**
 * Frees the memory occupied by a token list.
 *
 * @param token_list The token list to be freed.
 */
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

/**
 * Creates a new token node and adds it to a token list.
 *
 * @param type The type of the token.
 * @param data The data associated with the token.
 * @param token_list A pointer to the head of the token list.
 */
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
