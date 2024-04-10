/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:10:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/10 23:04:35 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"

static int	modify_env_var(t_env_list *env, const char *name, const char *value)
{
	t_env_list	*current;

	current = env;
	while (current)
	{
		if (!ft_strcmp(current->name, name))
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (1);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

static char	*getenv_no_alloc(t_env_list *env, char *name)
{
	t_env_list	*current;

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
	char		*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (0);
	}
	if (chdir(path) == -1)
	{
		free(cwd);
		perror("minishell: cd");
		return (1);
	}
	free(cwd);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	modify_env_var(master->env, "OLDPWD", getenv_no_alloc(master->env, "PWD"));
	modify_env_var(master->env, "PWD", cwd);
	free(cwd);
	return (0);
}

const char	*determine_target_path(int argc, char **argv, t_master *master)
{
	const char	*target_path;

	if (argc != 2 || ft_strncmp(argv[1], "--", 3) == 0)
	{
		target_path = getenv_no_alloc(master->env, "HOME");
		if (!target_path)
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	}
	else if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		target_path = getenv_no_alloc(master->env, "OLDPWD");
		if (!target_path)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		else
			ft_putendl_fd((char *)target_path, STDOUT_FILENO);
	}
	else
		target_path = argv[1];
	return (target_path);
}

int	builtin_cd(int argc, char **argv, t_master *master)
{
	const char	*target_path;

	if (argc > 2)
	{
		ft_putstr_fd(ESTR_CD_TOO_MANY_ARGS, STDERR_FILENO);
		return (1);
	}
	target_path = determine_target_path(argc, argv, master);
	if (!target_path)
		return (1);
	return (change_directory_and_update(master, target_path));
}
