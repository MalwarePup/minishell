/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:16:53 by ladloff           #+#    #+#             */
/*   Updated: 2023/06/30 18:35:25 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "execution.h"
#include "exit.h"
#include "libft.h"

char	*get_env_value(t_env *env, char *name)
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
					cleanup_executable();
					ft_error_exit("ft_strdup (get_env_value)", ENOMEM);
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

char	*extract_expansion_name(char *str)
{
	size_t	i;
	char	*name;

	i = 1;
	if (str[i] == '?')
	{
		name = ft_strdup("?");
		if (!name)
		{
			cleanup_executable();
			ft_error_exit("ft_strdup (extract_expansion_name)", ENOMEM);
		}
		return (name);
	}
	while (str[i] && str[i] != '$' && !ft_isspace(str[i]))
		i++;
	name = ft_strndup(str + 1, i - 1);
	if (!name)
	{
		cleanup_executable();
		ft_error_exit("ft_strndup (extract_expansion_name)", ENOMEM);
	}
	return (name);
}
