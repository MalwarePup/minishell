/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 10:13:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/19 14:05:54 by macbookpro       ###   ########.fr       */
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

int	create_token_node(t_cmd_type type, char **data, t_token **token)
{
	t_token	*new_node;
	char	*tmp;

	tmp = NULL;
	new_node = ft_calloc(1, sizeof(t_token));
	if (!new_node)
		return (EXIT_FAILURE);
	if (data == NULL || *data == NULL)
		return (create_node(NULL, &new_node, type, token), EXIT_SUCCESS);
	else
	{
		*data = trim_spaces(*data);
		if (!(*data))
			return (EXIT_FAILURE);
		tmp = ft_strdup(*data);
		if (!tmp)
			return (free(new_node), free(*data), EXIT_FAILURE);
		create_node(tmp, &new_node, type, token);
		return (free(*data), *data = NULL, EXIT_SUCCESS);
	}
}
