/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_mem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:17:03 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 18:43:45 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

void	free_double_ptr(char **str)
{
	char	**ptr;

	if (!str)
		return ;
	ptr = str;
	while (*ptr)
		free(*ptr++);
	free(str);
}

/**
 * Count the numbers of spaces in a string.
 *
 * @param str The string to be analyzed
 * @return The number of spaces found in the string
 */
static size_t	count_spaces(char *str)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (*str == ' ')
		{
			count++;
		}
		str++;
	}
	return (count);
}

t_exec	*create_arguments(t_master *master, t_token *token)
{
	t_exec	*new;

	new = ft_calloc(1, sizeof(t_exec));
	if (!new)
	{
		perror("ft_calloc in format_arg");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	if (token && token->data)
	{
		new->argv = malloc((count_spaces(token->data) + 2) * sizeof(char *));
		if (!new->argv)
		{
			perror("malloc in create_arguments");
			cleanup_before_exit(master);
			exit(EXIT_FAILURE);
		}
		split_args(master, token->data, new->argv, &new->simple_quotes);
		while (new->argv[new->argc])
			new->argc++;
	}
	return (new);
}
