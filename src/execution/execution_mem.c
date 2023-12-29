/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_mem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:17:03 by ladloff           #+#    #+#             */
/*   Updated: 2023/06/30 18:22:59 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "execution.h"
#include "exit.h"
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

t_exec	*create_arguments(t_token *token)
{
	t_exec	*new;

	new = ft_calloc(1, sizeof(t_exec));
	if (!new)
	{
		perror("ft_calloc in format_arg");
		cleanup_before_exit();
		exit(EXIT_FAILURE);
	}
	if (token && token->data)
	{
		new->argv = malloc((count_spaces(token->data) + 2) * sizeof(char *));
		if (!new->argv)
		{
			perror("malloc in create_arguments");
			cleanup_before_exit();
			exit(EXIT_FAILURE);
		}
		split_args(token->data, new->argv);
		while (new->argv[new->argc])
			new->argc++;
	}
	return (new);
}
