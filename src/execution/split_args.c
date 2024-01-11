/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:22:32 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 21:50:51 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

static char	*allocate_memory_for_arg(t_master *master, char *s)
{
	char	*arg;

	arg = malloc(ft_strlen(s) + 1);
	if (!arg)
	{
		perror("malloc in split_args");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	arg[0] = '\0';
	return (arg);
}

static char	*clean_arg(char *s)
{
	int		i;
	int		j;
	char	*news;
	int		backslash_count;

	i = 0;
	j = 0;
	backslash_count = 0;
	while (s[i])
	{
		if (s[i] == '\\' && (s[i + 1] == '"' || s[i + 1] == '\''))
			backslash_count++;
		i++;
	}
	news = calloc(1, ft_strlen(s) - backslash_count + 1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\\' && (s[i + 1] == '"' || s[i + 1] == '\''))
			i++;
		news[j++] = s[i++];
	}
	news[j] = '\0';
	free(s);
	return (news);
}

static char	*handle_quoted_argument(char *s, char **arg)
{
	char	quote;
	char	*start;

	quote = *s++;
	start = s;
	while (*s)
	{
		if (*s == quote)
			if ((*(s - 1) != '\\') || (*(s - 1) == '\\' && *(s - 2) == '\\'))
				break ;
		s++;
	}
	ft_strncat(*arg, start, s - start);
	if (*s)
		s++;
	return (s);
}

static char	*handle_unquoted_argument(char *s, char **arg)
{
	ft_strncat(*arg, s, 1);
	s++;
	return (s);
}

void	split_args(t_master *master, char *s, char **argv,
	bool *is_simple_quotes)
{
	char	*arg;

	if (!s || !argv)
		return ;
	while (*s)
	{
		arg = allocate_memory_for_arg(master, s);
		while (*s && *s != ' ')
		{
			if (((*s == '\'') || (*s == '\"')) && !is_escaped(s, s - s))
			{
				if (*s == '\'')
					*is_simple_quotes = true;
				s = handle_quoted_argument(s, &arg);
			}
			else
				s = handle_unquoted_argument(s, &arg);
		}
		arg = clean_arg(arg);
		*argv++ = arg;
		while (*s == ' ')
			s++;
	}
	*argv = NULL;
}
