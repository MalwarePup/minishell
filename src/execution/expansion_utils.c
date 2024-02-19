/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:16:53 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/19 12:58:12 by macbookpro       ###   ########.fr       */
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

bool  is_valid_expansion_name(const char *name)
{
    const char *p = name;

    if (!name || !*name)
        return (true);
    if (ft_isdigit(name[0]))
        return (false);
    while (*p)
    {
        if (!ft_isalnum(*p) && *p != '_' && *p != '\"' && *p != '\'')
            return (false);
        p++;
    }
    return (true);
}
