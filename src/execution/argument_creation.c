/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_creation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:17:03 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/09 18:07:04 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

static size_t	count_argc(char *s)
{
	size_t	i;
	size_t	count;
	char	c;

	i = 0;
	count = 0;
	c = 0;
	if (s[i] != ' ' && s[i] != '\0')
		count++;
	i++;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				i++;
			count++;
		}
		else if (s[i] != ' ' && (s[i - 1] == ' ' || s[i - 1] == '"' || s[i - 1] == '\''))
			count++;
		i++;
	}
	return (count + 1);
}
size_t	count_argc_for_echo(char *s)
{
	size_t	i;
	size_t	count;
	char	c;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
				i++;
			count++;
			if (s[i])
				i++;
		}
		else
		{
			while (s[i] != '\'' && s[i] !=  '"' && s[i])
				i++;
			count++;
			if (s[i])
				i++;
		}
	}
	return (count + 2);
}

bool	is_echo(t_master *master, t_token *token)
{
	int	i;

	i = 0;
	(void)master;
	while (ft_isspace(token->data[i]))
		i++;
	if (!ft_strncmp(token->data + i, "echo ", 5))
		return (true);
	return (false);
}

int	split_args(t_master *master, char *s, char **argv, bool echo)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s || !argv)
		return (0);
	while (s[j])
	{
		if (!echo)
		{
			while (ft_isspace(s[j]) && s[j])
				j++;
		}
		if ((s[j] == '\'' || s[j] == '"' ) && is_not_escaped(s, j))
			argv[i] = creates_quoted_arg(master, s, &j);
		else if (!echo)
			argv[i] = creates_arg(master, s, &j);
		else
		{
			argv[i] = creates_arg_for_echo(master, s, &j);
		}
		i++;
	}
	argv[i] = NULL;
	return (i);
}

void	create_arguments(t_master *master, t_token *token)
{
	bool	echo;
	int		argc;

	echo = is_echo(master, token);
	if (token && token->data && token->type)
	{
		if (echo)
			argc = count_argc_for_echo(token->data);
		else
			argc = count_argc(token->data);
		master->exec->argv = malloc(argc * sizeof(char *));
		if (!master->exec->argv)
		{
			perror("malloc in create_arguments");
			cleanup_before_exit(master);
			exit(EXIT_FAILURE);
		}
		master->exec->argc = split_args(master, token->data,
				master->exec->argv, echo);
	}
}

