/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_splitting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:22:32 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/07 13:11:52 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

bool	is_not_escaped(char *s, int index)
{
	if (index == 0)
		return (true);
	else if (index == 1)
		return (s[index - 1] != '\\');
	else
		return (s[index - 1] != '\\' || s[index - 2] == '\\');
}

char	*creates_quoted_arg(t_master *master, char *s, size_t *j)
{
	char	quote;
	char	*arg;
	size_t	i;

	i = 0;
	quote = s[*j];
	arg = calloc(ft_strlen(s) + 1 - *j, sizeof(char));
	if (!arg)
	{
		perror("malloc in split_args");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	arg[i++] = quote;
	while (s[*j + i] != '\0' && (s[*j + i] != quote
			|| (!is_not_escaped(s, *j + i) && s[*j + i] == quote)))
	{
		arg[i] = s[*j + i];
		i++;
	}
	arg[i++] = quote;
	arg[i] = '\0';
	*j = *j + i;
	return (arg);
}

char	*creates_arg(t_master *master, char *s, size_t *j)
{
	size_t	i;
	char	*arg;

	i = 0;
	arg = calloc(ft_strlen(s) + 1 - *j, sizeof(char));
	if (!arg)
	{
		perror("malloc in split_args");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	while (s[*j] && ft_isspace(s[*j]))
		*j = *j + 1;
	while (s[*j + i] != '\0' && !ft_isspace(s[*j + i]) && ((s[*j + i] != '"'
				&& s[*j + i] != '\'') || (!is_not_escaped(s, *j + i)
				&& (s[*j + i] == '"' || s[*j + i] == '\''))))
	{
		arg[i] = s[*j + i];
		i++;
	}
	arg[i] = '\0';
	*j = *j + i;
	while (s[*j] && ft_isspace(s[*j]))
		*j = *j + 1;
	return (arg);
}

// static char *allocate_memory_for_arg(t_master *master, char *s)
// {
// 	char *arg;

// 	arg = malloc(ft_strlen(s) + 1);
// 	if (!arg)
// 	{
// 		perror("malloc in split_args");
// 		cleanup_before_exit(master);
// 		exit(EXIT_FAILURE);
// 	}
// 	arg[0] = '\0';
// 	return (arg);
// }


// static char *handle_quoted_argument(char *s, char **arg)
// {
// 	char quote;
// 	char *start;

// 	quote = *s++;
// 	start = s;
// 	while (*s)
// 	{
// 		if (*s == quote)
// 			if ((*(s - 1) != '\\') || (*(s - 1) == '\\' && *(s - 2) == '\\'))
// 				break;
// 		s++;
// 	}
// 	ft_strncat(*arg, start, s - start);
// 	if (*s)
// 		s++;
// 	return (s);
// }

// static char *handle_unquoted_argument(char *s, char **arg)
// {
// 	ft_strncat(*arg, s, 1);
// 	s++;
// 	return (s);
// }

// int split_args(t_master *master, char *s, char **argv,
// 			   bool *is_simple_quotes)
// {
// 	char *arg;
// 	int i;

// 	i = 0;
// 	if (!s || !argv)
// 		return (0);
// 	while (*s)
// 	{
// 		arg = allocate_memory_for_arg(master, s);
// 		while (*s && !ft_isspace(*s))
// 		{
// 			if (*s == '\'' && !is_escaped(s, s - s))
// 				*is_simple_quotes = true;
// 			if (((*s == '\'') || (*s == '\"')) && !is_escaped(s, s - s))
// 				s = handle_quoted_argument(s, &arg);
// 			else
// 				s = handle_unquoted_argument(s, &arg);
// 		}
// 		arg = clean_arg(arg);
// 		argv[i++] = arg;
// 		while (ft_isspace(*s))
// 			s++;
// 	}
// 	argv[i] = NULL;
// 	return (i);
// }

