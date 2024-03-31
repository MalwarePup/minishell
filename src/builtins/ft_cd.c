/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:10:38 by ladloff           #+#    #+#             */
/*   Updated: 2024/03/31 16:10:33 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"

static int	update_var_pwd(t_env_list *env)
{
	t_env_list	*current;
	char		*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	current = env;
	while (current && current->name && ft_strcmp(current->name, "PWD"))
		current = current->next;
	if (!current)
	{
		free(cwd);
		return (1);
	}
	free(current->value);
	current->value = ft_strdup(cwd);
	if (!current->value)
		return (1);
	free(cwd);
	return (EXIT_SUCCESS);
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
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	if (update_var_pwd(master->env))
		return (1);
	return (EXIT_SUCCESS);
}

int	ft_cd(int argc, char **argv, t_master *master)
{
	const char	*home_path;

	if (argc > 2)
	{
		ft_putstr_fd(ESTR_CD_TOO_MANY_ARGS, STDERR_FILENO);
		return (1);
	}
	else if (argc != 2 || ft_strncmp(argv[1], "--", 3) == 0)
	{
		home_path = getenv_no_alloc(master->env, "HOME");
		if (!home_path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
		return (change_directory_and_update(master, home_path));
	}
	else if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		home_path = getenv_no_alloc(master->env, "OLDPWD");
		if (!home_path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			return (1);
		}
		ft_putendl_fd((char *)home_path, STDOUT_FILENO);
		return (change_directory_and_update(master, home_path));
	}
	else if (ft_strncmp(argv[1], "---", 4) == 0)
	{
		ft_putstr_fd("minishell: usage: cd [-L|[-P [-e]] [-@]] [dir]\n",
			STDERR_FILENO);
		return (EXIT_MISUSE);
	}
	else
		return (change_directory_and_update(master, (const char *)argv[1]));
}
