/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:34:53 by alfloren          #+#    #+#             */
/*   Updated: 2024/03/09 21:55:08 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"
#include <stdio.h>
#include <errno.h>

int	two_consecutive_pipe(t_master *master)
{
	t_token	*tmp;

	tmp = master->token;
	while (tmp)
	{
		if ((tmp)->type == CMD_PIPE
			&& (tmp)->next && (tmp)->next->type == CMD_PIPE)
		{
			master->exit_status = EXIT_MISUSE;
			ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '|');
			return (1);
		}
		tmp = (tmp)->next;
	}
	return (EXIT_SUCCESS);
}

void	lexer_exit(t_master *master, t_lexer *lexer, char *str)
{
	clean_lexer(lexer);
	free_token(&master->token);
	error_exit(master, str);
}

int	last_operator(t_master *master)
{
	if (master->token && master->token->last
		&& master->token->last->type == CMD_PIPE)
		return (ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '|'),
			master->exit_status = EXIT_MISUSE, 1);
	return (EXIT_SUCCESS);
}
