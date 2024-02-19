/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:16:53 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/19 10:31:52 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

char	*getenv_value(t_master *master, t_env *env, char *name)
{
	char	*value;

	if (!env || !env->name || !name)
		return (NULL);
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			if (env->value)
			{
				value = ft_strdup(env->value);
				if (!value)
				{
					free(name);
					ft_error_exit(master, "ft_strdup (get_env_value)", ENOMEM);
				}
				return (value);
			}
			else
				return (NULL);
		}
		env = env->next;
	}
	return (NULL);
}

char	*extract_expansion_name(t_master *master, char *str)
{
	size_t	i;
	char	*name;

	i = 1;
	if (str[i] == '?')
	{
		name = ft_strdup("?");
		if (!name)
			ft_error_exit(master, "ft_strdup (extract_expansion_name)", ENOMEM);
		return (name);
	}
	while (str[i] && str[i] != '$' && !ft_isspace(str[i])
		&& str[i] != '\'' && str[i] != '"')
		i++;
	name = ft_strndup(str + 1, i - 1);
	if (!name)
		ft_error_exit(master, "ft_strndup (extract_expansion_name)", ENOMEM);
	return (name);
}

bool	is_inside_double_quotes(char *str, size_t pos)
{
	size_t	i;
	bool	inside_double_quotes;

	i = 0;
	inside_double_quotes = false;
	while (i < pos)
	{
		if (str[i] == '"')
			inside_double_quotes = !inside_double_quotes;
		i++;
	}
	return (inside_double_quotes);
}

size_t	count_single_quotes_until_pos(char *str, size_t pos)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (i < pos)
	{
		if (str[i] == '\'')
			count++;
		i++;
	}
	return (count);
}
