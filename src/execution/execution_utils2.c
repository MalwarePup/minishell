/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:32:49 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/12 14:35:19 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

int	count_pipe(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->type == CMD_PIPE)
			i++;
		token = token->next;
	}
	return (i + 1);
}
