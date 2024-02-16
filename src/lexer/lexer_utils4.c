/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:34:53 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/16 13:13:01 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"
#include <stdio.h>
#include <errno.h>

char	*trim_spaces(char *str)
{
	size_t	i;
	size_t	j;
	char	*new_str;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	while (str[i] && ft_isspace(str[i]))
		i++;
	while (str[i + j])
		j++;
	while (j > 0 && ft_isspace(str[i + j - 1]))
		j--;
	new_str = malloc(j + 1);
	if (!new_str)
		return (free(str), NULL);
	ft_strlcpy(new_str, str + i, j + 1);
	free(str);
	return (new_str);
}

int	two_consecutive_pipe(t_token **token)
{
	t_token	*tmp;

	tmp = *token;
	while (tmp)
	{
		if ((tmp)->type == CMD_PIPE
			&& (tmp)->next && (tmp)->next->type == CMD_PIPE)
		{
			printf(ESTR_UNEXP, '|');
			return (free_token(token), EXIT_FAILURE);
		}
		tmp = (tmp)->next;
	}
	return (EXIT_SUCCESS);
}

int	exit_handler(t_master *master, t_token **token)
{
	int	i;

	if (*token == NULL || start_operator(master, (*token)->type, token))
		return (EXIT_FAILURE);
	if (two_consecutive_pipe(token) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = EXIT_FAILURE;
	if (!(*token))
		return (i);
	if ((*token)->type == CMD_OTHERS)
		i = EXIT_SUCCESS;
	else if ((*token)->type == CMD_PIPE)
		printf(ESTR_UNEXP, '|');
	else if ((*token)->type == CMD_RED_IN || (*token)->type == CMD_RED_OUT
		|| (*token)->type == CMD_D_RED_IN
		|| (*token)->type == CMD_D_RED_OUT)
		i = EXIT_SUCCESS;
	else if ((*token)->type != CMD_OTHERS)
	{
		master->exit_status = 2;
		printf(ESTR_OPSTART_P1 ESTR_OPSTART_P2);
	}
	if (i || is_clean(token) || is_heredoc_pipe(token))
		return (free_token(token), master->exit_status = 2, EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
