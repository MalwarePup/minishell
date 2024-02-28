/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:16:53 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/28 16:09:22 by ladloff          ###   ########.fr       */
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
					error_exit(master, "ft_strdup (get_env_value)");
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
	if (str[i] == '?' || ft_isdigit(str[i]))
	{
		i++;
		name = ft_strndup((str + i - 1), 1);
		if (!name)
			error_exit(master, "ft_strndup (expansion)");
		return (name);
	}
	while (str[i] && !ft_isspace(str[i])
		&& (ft_isalnum(str[i]) || (!ft_isalnum(str[i]) && str[i] == '_')))
		i++;
	name = ft_strndup(str + 1, i - 1);
	if (!name)
		error_exit(master, "ft_strndup (extract_expansion_name)");
	return (name);
}
