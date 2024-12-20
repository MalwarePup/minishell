/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_creation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:17:03 by ladloff           #+#    #+#             */
/*   Updated: 2024/11/05 18:47:02 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

static size_t	count_words(char *s)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]) && s[i])
			i++;
		if (s[i])
			count++;
		while (!ft_isspace(s[i]) && s[i])
			i++;
	}
	return (count);
}

static char	*create_arg(t_master *master, char *str, size_t *j)
{
	char	*arg;
	char	quote;
	size_t	i;

	arg = malloc(sizeof(char) * (ft_strlen(str) + 1));
	quote = 0;
	i = 0;
	if (!arg)
	{
		perror("malloc in create_arguments");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	while (is_valid_character(str[*j], false, &quote))
	{
		arg[i] = str[(*j)];
		(*j)++;
		i++;
	}
	arg[i] = '\0';
	return (arg);
}

static size_t	split_args(t_master *master, char *s, char **argv)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s || !argv)
		return (0);
	while (s[j])
	{
		while (ft_isspace(s[j]) && s[j])
			j++;
		if (s[j])
			argv[i++] = create_arg(master, s, &j);
	}
	return (i);
}

void	create_arguments(t_master *master, t_token *token)
{
	size_t	argc;

	argc = count_words(token->data) + 1;
	if (token && token->data && token->type)
	{
		master->argv = ft_calloc(argc, sizeof(char *));
		if (!master->argv)
		{
			perror("malloc in create_arguments");
			cleanup_before_exit(master);
			exit(EXIT_FAILURE);
		}
		master->argc = split_args(master, token->data,
				master->argv);
	}
}
