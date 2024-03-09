/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 12:42:17 by ladloff           #+#    #+#             */
/*   Updated: 2024/03/10 00:18:10 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int	update_shlvl(char **dst, int old_value)
{
	free(*dst);
	*dst = ft_itoa(old_value + 1);
	if (!*dst)
	{
		perror("ft_itoa (update_shlvl)");
		return (1);
	}
	return (0);
}

static int	create_add_env_node(t_env_list **env, char *name, char *value)
{
	t_env_list	*new_node;

	new_node = malloc(sizeof(t_env_list));
	if (!new_node)
	{
		free(name);
		free(value);
		perror("malloc (create_add_env_node)");
		return (1);
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = NULL;
	if (!*env)
	{
		*env = new_node;
		(*env)->last = new_node;
	}
	else
	{
		(*env)->last->next = new_node;
		(*env)->last = new_node;
	}
	return (0);
}

static int	manage_pwd(t_env_list **env)
{
	char	*cwd;
	char	*name;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd (manage_empty_environment)");
		return (1);
	}
	name = ft_strdup("PWD");
	if (!name)
	{
		free(cwd);
		perror("ft_strdup (manage_empty_environment)");
		return (1);
	}
	if (create_add_env_node(env, name, cwd))
		return (1);
	return (0);
}

static int	manage_empty_environment(t_env_list **env)
{
	char	*name;
	char	*value;

	if (manage_pwd(env))
		return (1);
	name = ft_strdup("SHLVL");
	value = ft_strdup("1");
	if (!name || !value)
	{
		free(name);
		free(value);
		perror("ft_strdup (manage_empty_environment)");
		return (1);
	}
	if (create_add_env_node(env, name, value))
		return (1);
	name = ft_strdup("_");
	value = ft_strdup("minishell");
	if (!name || !value)
	{
		free(name);
		free(value);
		perror("ft_strdup (manage_empty_environment)");
		return (1);
	}
	if (create_add_env_node(env, name, value))
		return (1);
	return (0);
}

int	manage_environment(t_env_list **env)
{
	char		*name;
	char		*value;
	char		*equals_location;
	extern char	**environ;

	if (!*environ)
		if (manage_empty_environment(env))
			return (1);
	while (*environ)
	{
		equals_location = ft_strchr(*environ, '=');
		name = ft_strndup(*environ, equals_location - *environ);
		if (!name)
		{
			perror("ft_strndup (manage_environment)");
			return (1);
		}
		value = ft_strdup(equals_location + 1);
		if (!value)
		{
			free(name);
			perror("ft_strdup (manage_environment)");
			return (1);
		}
		if (!ft_strcmp(name, "SHLVL"))
		{
			if (update_shlvl(&value, ft_atoi(value)))
			{
				free(name);
				return (1);
			}
		}
		if (create_add_env_node(env, name, value))
			return (1);
		environ++;
	}
	return (0);
}
