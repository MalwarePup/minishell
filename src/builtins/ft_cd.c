/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:10:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 19:21:24 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"

static int	update_var_pwd(t_env *env_list)
{
	t_env	*current;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (EXIT_FAILURE);
	}
	current = env_list;
	while (current && current->name && ft_strcmp(current->name, "PWD"))
		current = current->next;
	if (!current)
	{
		free(cwd);
		return (EXIT_FAILURE);
	}
	free(current->value);
	current->value = ft_strdup(cwd);
	if (!current->value)
		return (EXIT_FAILURE);
	free(cwd);
	return (EXIT_SUCCESS);
}

static char	*get_env_value_no_alloc(t_env *env, char *name)
{
	t_env	*current;

	current = env;
	if (!current || !current->name)
		return (NULL);
	while (current)
	{
		if (!ft_strcmp(current->name, name))
		{
			if (current->value)
				return (current->value);
			else
				return (NULL);
		}
		current = current->next;
	}
	return (NULL);
}

static int	change_directory_and_update(t_master *master, const char *path)
{
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (EXIT_FAILURE);
	}
	if (update_var_pwd(master->env_list) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_cd(int argc, char **argv, t_master *master)
{
	const char	*home_path;

	if (argc != 2)
	{
		home_path = get_env_value_no_alloc(master->env_list, "HOME");
		if (!home_path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		return (change_directory_and_update(master, home_path));
	}
	return (change_directory_and_update(master, (const char *)argv[1]));
}
