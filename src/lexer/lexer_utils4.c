/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:34:53 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/15 18:19:56 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"
#include <stdio.h>
#include <errno.h>

t_cmd_type	redir_type(char *line_read, size_t *i)
{
	char		redir;
	t_cmd_type	type;

	redir = line_read[(*i)++];
	if (line_read[(*i)] == redir)
	{
		if (line_read[(*i) + 1] != redir)
		{
			if (redir == '>')
				type = CMD_D_RED_OUT;
			else
				type = CMD_D_RED_IN;
			(*i)++;
		}
		else
			return (ft_dprintf(STDOUT_FILENO, ESTR_UNEXP, redir), CMD_ERROR);
	}
	else
	{
		if (redir == '>')
			type = CMD_RED_OUT;
		else
			type = CMD_RED_IN;
	}
	return (type);
}

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

int	exit_handler(t_master *master, t_token **token)
{
	int	i;

	if (start_operator((*token)->type))
		return (EXIT_FAILURE);
	i = EXIT_FAILURE;
	if (!(*token))
		return (i);
	if ((*token)->type == CMD_OTHERS)
		i = EXIT_SUCCESS;
	else if ((*token)->type == CMD_PIPE)
		printf(ESTR_UNEXP, '|');
	else if ((*token)->type == CMD_RED_IN
		|| (*token)->type == CMD_RED_OUT
		|| (*token)->type == CMD_D_RED_IN
		|| (*token)->type == CMD_D_RED_OUT)
		i = EXIT_SUCCESS;
	else if ((*token)->type != CMD_OTHERS)
		printf(ESTR_OPSTART_P1 ESTR_OPSTART_P2);
	if (i || is_clean(token) || is_heredoc_pipe(token))
	{
		master->exit_status = 2;
		return (free_token(token), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
