/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:16:53 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/10 13:20:43 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"

char	*get_env_value(t_master *master, t_env *env, char *name)
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
					ft_error_exit(master, "ft_strdup (get_env_value)", ENOMEM,
						true);
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
			ft_error_exit(master, "ft_strdup (extract_expansion_name)", ENOMEM,
				true);
		return (name);
	}
	while (str[i] && str[i] != '$' && !ft_isspace(str[i]) && str[i] != '\'')
		i++;
	name = ft_strndup(str + 1, i - 1);
	if (!name)
		ft_error_exit(master, "ft_strndup (extract_expansion_name)", ENOMEM,
			true);
	return (name);
}

int	replace_argv_without_quotes(t_master *master, t_expansion *exp)
{
	char	*new_str;
	char	quote;

	if (master->exec->argv[exp->i][0] == '\"'
		|| master->exec->argv[exp->i][0] == '\'')
	{
		quote = master->exec->argv[exp->i][0];
		new_str = ft_strdup(master->exec->argv[exp->i] + 1);
		if (!new_str)
			ft_error_exit(master, "ft_strdup (replace_argv_without_quotes)",
				ENOMEM, true);
		new_str[ft_strlen(new_str) - 1] = '\0';
		free(master->exec->argv[exp->i]);
		master->exec->argv[exp->i] = new_str;
		return (quote == '\"');
	}
	return (1);
}
