/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:34:53 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/20 17:55:16 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"
#include <stdio.h>
#include <errno.h>

char	*trim_spaces(t_master *master, t_lexer *lexer, char *str)
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
	if (!(str[i]))
		return (NULL);
	while (str[i + j])
		j++;
	while (j > 0 && ft_isspace(str[i + j - 1]))
		j--;
	new_str = malloc(j + 1);
	if (!new_str)
		return (free(str),
			lexer_exit(master, lexer, "malloc trimspaces"), NULL);
	ft_strlcpy(new_str, (str + i), j + 1);
	free(str);
	return (new_str);
}

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
			return (EXIT_FAILURE);
		}
		tmp = (tmp)->next;
	}
	return (EXIT_SUCCESS);
}

int	exit_handler(t_master *master, t_token **token)
{
	int	i;

	if (*token == NULL)
		return (EXIT_FAILURE);
	if (two_consecutive_pipe(master) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = EXIT_FAILURE;
	if (!(*token))
		return (i);
	if ((*token)->type == CMD_OTHERS)
		i = EXIT_SUCCESS;
	else if ((*token)->type == CMD_PIPE || (*token)->last->type == CMD_PIPE)
		printf(ESTR_UNEXP, '|');
	else if ((*token)->type == CMD_RED_IN || (*token)->type == CMD_RED_OUT
		|| (*token)->type == CMD_D_RED_IN
		|| (*token)->type == CMD_D_RED_OUT)
		i = EXIT_SUCCESS;
	else if ((*token)->type != CMD_OTHERS)
		printf(ESTR_OPSTART_P1 ESTR_OPSTART_P2);
	if (i || is_clean(token) || is_heredoc_pipe(token))
	{
		master->exit_status = EXIT_MISUSE;
		return (free_token(token), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	lexer_exit(t_master *master, t_lexer *lexer, char *str)
{
	clean_lexer(lexer);
	free_token(&master->token);
	master->token = NULL;
	perror(str);
}

int	last_operator(t_master *master)
{
	if (master->token && master->token->last
		&& master->token->last->type == CMD_PIPE)
		return (ft_dprintf(STDERR_FILENO, ESTR_UNEXP, '|'),
			master->exit_status = EXIT_MISUSE, EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
