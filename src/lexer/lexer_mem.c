/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:13:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/14 12:33:47 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"

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
