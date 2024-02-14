/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:41:29 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/14 12:56:22 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "ft_dprintf.h"
#include "libft.h"

char	*trim_spaces(t_master *master, const char *str, size_t start,
	size_t end)
{
	size_t		i;
	size_t		length;
	char		*trimmed_str;
	const char	ops[4] = "|&<>";

	if (start >= ft_strlen(str) || end >= ft_strlen(str))
		return (NULL);
	i = 0;
	while (i < 4 && ops[i])
	{
		if (ops[i] == str[end])
			end--;
		i++;
	}
	while (ft_isspace(str[start]) && start <= end)
		start++;
	while (ft_isspace(str[end]) && end >= start)
		end--;
	length = end - start + 1;
	trimmed_str = malloc((length + 1) * sizeof(char));
	if (!trimmed_str)
		error_exit(master, "malloc error in trim_spaces", false);
	ft_strlcpy(trimmed_str, &str[start], length + 1);
	return (trimmed_str);
}

static int	is_heredoc_pipe(t_token **token)
{
	t_token	*current;

	current = *token;
	while (current)
	{
		if (current->type == CMD_PIPE && !current->next)
		{
			free_token(token);
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

static int	is_clean(t_token **token)
{
	char		type;
	t_token		*current;
	const char	*ops[OP] = {"|", "<", "<<", ">", ">>"};

	current = *token;
	while (current && current->next)
	{
		if (current->next->type == CMD_OTHERS && !ft_strlen(current->next->data)
			&& current->type > CMD_OTHERS)
		{
			type = *ops[current->type - 1];
			printf(ESTR_UNEXP, type);
			free_token(token);
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	exit_handler(t_master *master, t_token **token)
{
	int	i;

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
