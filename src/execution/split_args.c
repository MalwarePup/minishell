/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:22:32 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/05 07:14:42 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "execution.h"
#include "exit.h"
#include "libft.h"

static char	*allocate_memory_for_arg(char *s)
{
	char	*arg;

	arg = malloc(ft_strlen(s) + 1);
	if (!arg)
	{
		perror("malloc in split_args");
		cleanup_before_exit();
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

void	split_args(char *s, char **argv)
{
	char	*arg;
	int		argc;

	if (!s || !argv)
		return ;
	argc = 0;
	while (*s)
	{
		arg = allocate_memory_for_arg(s);
		while (*s && *s != ' ')
		{
			if (((*s == '\'') || (*s == '\"')) && (*(s - 1) != '\\'))
				s = handle_quoted_argument(s, &arg);
			else
				s = handle_unquoted_argument(s, &arg);
		}
		arg = clean_arg(arg);
		argv[argc++] = arg;
		while (*s == ' ')
			s++;
	}
	argv[argc] = NULL;
}
