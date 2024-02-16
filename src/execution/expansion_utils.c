/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 12:16:53 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 21:17:35 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

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
		&& str[i] != '\'' && str[i] != '\"')
		i++;
	name = ft_strndup(str + 1, i - 1);
	if (!name)
		ft_error_exit(master, "ft_strndup (extract_expansion_name)", ENOMEM);
	return (name);
}

int	to_pass(char *str, char *quote, char *ex_quote, size_t *i)
{
	*ex_quote = *quote;
	condition_while(str, *i, true, quote);
	if (*ex_quote != *quote)
	{
		(*i)++;
		return (true);
	}
	return (false);
}

int	replace_argv_without_quotes(t_master *master, t_expansion *exp)
{
	char	*new_str;
	char	quote;
	char	ex_quote;
	size_t	ij[2];

	quote = 0;
	ex_quote = 0;
	ij[0] = 0;
	ij[1] = 0;
	new_str = malloc(sizeof(char)
			* (ft_strlen(master->argv[exp->i]) + 1));
	if (!new_str)
		return (EXIT_FAILURE);
	while (master->argv[exp->i][ij[0]])
	{
		if (to_pass(master->argv[exp->i], &quote, &ex_quote, &ij[0]))
			continue ;
		new_str[ij[1]++] = master->argv[exp->i][ij[0]++];
	}
	new_str[ij[1]] = '\0';
	free(master->argv[exp->i]);
	master->argv[exp->i] = new_str;
	return (EXIT_SUCCESS);
}

int	replace_redir_without_quotes(char **str)
{
	char	*new_str;
	char	*test;
	char	quote;
	char	ex_quote;
	size_t	ij[2];

	quote = 0;
	ex_quote = 0;
	ij[0] = 0;
	ij[1] = 0;
	test = malloc(sizeof(char) * (ft_strlen(*str) + 1));
	ft_strlcpy(test, *str, ft_strlen(*str) + 1);
	new_str = malloc(sizeof(char)
			* (ft_strlen(*str) + 1));
	if (!new_str)
		return (free(*str), EXIT_FAILURE);
	while (test[ij[0]])
	{
		if (to_pass(test, &quote, &ex_quote, &ij[0]))
			continue ;
		new_str[ij[1]++] = test[ij[0]++];
	}
	new_str[ij[1]] = '\0';
	return (free(*str), *str = new_str, free(test), EXIT_SUCCESS);
}
