/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_creation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:17:03 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/09 10:34:49 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

static size_t	count_spaces(char *str)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (*str == ' ')
			count++;
		str++;
	}
	return (count);
}

int	split_args(t_master *master, char *s, char **argv)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s || !argv)
		return (0);
	while (s[j])
	{
		if ((s[j] == '\'' || s[j] == '"' ) && is_not_escaped(s, j))
			argv[i] = creates_quoted_arg(master, s, &j);
		else
			argv[i] = creates_arg(master, s, &j);
		i++;
	}
	argv[i] = NULL;
	return (i);
}

void	create_arguments(t_master *master, t_token *token)
{
	if (token && token->data && token->type)
	{
		master->exec->argv = malloc((count_spaces(token->data) + 2)
				* sizeof(char *));
		if (!master->exec->argv)
		{
			perror("malloc in create_arguments");
			cleanup_before_exit(master);
			exit(EXIT_FAILURE);
		}
		master->exec->argc = split_args(master, token->data,
				master->exec->argv);
	}
}

// static size_t	count_argc(char *s)
// {
// 	size_t	i;
// 	size_t	count;
// 	char	c;

// 	i = 0;
// 	count = 0;
// 	c = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '\'' || s[i] == '"')
// 		{
// 			c = s[i];
// 			i++;
// 			while (s[i] && s[i] != c)
// 				i++;
// 			count++;
// 		}
// 		else if (s[i] == ' ' && s[i + 1] != ' ' && s[i + 1]
// 			!= '\0' && s[i + 1] != '\'' && s[i + 1] != '"')
// 			count++;
// 		i++;
// 	}
// 	return (count + 1);
// }
