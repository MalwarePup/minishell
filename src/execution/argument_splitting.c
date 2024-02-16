/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_splitting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:22:32 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 13:47:36 by ladloff          ###   ########.fr       */
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

char	*create_echo_arg(char *arg, size_t *j)
{
	arg = ft_strncat(arg, "echo", 4);
	*j = 5;
	arg[4] = '\0';
	return (arg);
}

char	*creates_quoted_arg(t_master *master, char *s, size_t *j)
{
	char	quote;
	char	*arg;
	size_t	i;

	i = 0;
	quote = s[*j];
	arg = ft_calloc(ft_strlen(s) + 1 - *j, sizeof(char));
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
	arg = ft_calloc(ft_strlen(s) + 1 - *j, sizeof(char));
	if (!arg)
	{
		perror("malloc in split_args");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	while (s[*j + i] != '\0' && !ft_isspace(s[*j + i]) && ((s[*j + i] != '"'
				&& s[*j + i] != '\'') || (!is_not_escaped(s, *j + i)
				&& (s[*j + i] == '"' || s[*j + i] == '\''))))
	{
		arg[i] = s[*j + i];
		i++;
	}
	arg[i] = '\0';
	*j = *j + i;
	return (arg);
}

char	*creates_arg_for_echo(t_master *master, char *s, size_t *j)
{
	size_t	i;
	char	*arg;

	i = 0;
	arg = ft_calloc(ft_strlen(s) + 1 - *j, sizeof(char));
	if (!arg)
	{
		perror("malloc in split_args");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	if (!ft_strncmp(s + *j, "echo ", 5) && *j == 0)
		return (create_echo_arg(arg, j));
	while (s[*j + i] != '\0' && ((s[*j + i] != '"'
				&& s[*j + i] != '\'') || (!is_not_escaped(s, *j + i)
				&& (s[*j + i] == '"' || s[*j + i] == '\''))))
	{
		arg[i] = s[*j + i];
		i++;
	}
	arg[i] = '\0';
	*j = *j + i;
	return (arg);
}
