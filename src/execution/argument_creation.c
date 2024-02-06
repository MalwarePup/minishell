/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_creation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:17:03 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/06 19:20:20 by alfloren         ###   ########.fr       */
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

static size_t	count_argc(char *s)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == '\'')
		{
			i++;
			while (s[i] && s[i] != '\'')
				i++;
			count++;
		}
		else if (s[i] == '"')
		{
			i++;
			while (s[i] && s[i] != '"')
				i++;
			count++;
		}
		else if (s[i] == ' ' && s[i + 1] != ' ' && s[i + 1]
			!= '\0' && s[i + 1] != '\'' && s[i + 1] != '"')
			count++;
		i++;
	}
	return (count + 1);
}

void	create_arguments(t_master *master, t_token *token)
{
	if (token && token->data && token->type)
	{
		// printf("count_argc = %zu\n", count_argc(token->data));
		// printf("count_spaces = %zu\n", count_spaces(token->data) + 2);
		master->exec->argv = malloc((count_spaces(token->data) + 2)
				* sizeof(char *));
		if (!master->exec->argv)
		{
			perror("malloc in create_arguments");
			cleanup_before_exit(master);
			exit(EXIT_FAILURE);
		}
		master->exec->argc = count_argc(token->data);
		split_args(master, token->data, master->exec->argv,
			&(master->exec->simple_quotes));
		// printf("master->exec->argc = %d\n", master->exec->argc);
		// while (master->exec->argv[master->exec->argc])
		// {
		// 	printf("master->exec->argc = %d\n" , master->exec->argc);
		// 	master->exec->argc++;
		// }
	}
}
