/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:09 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 19:04:12 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "minishell.h"
#include "libft.h"

static t_builtin_type	inspect_token(char *arg)
{
	size_t					i;
	t_builtin_type			type;
	const char				*builtins[8] = {"cd", "echo", "env", "exit", "pwd",
		"unset", "export", "exit"};
	const t_builtin_type	types[8] = {T_CD, T_ECHO, T_ENV, T_EXIT, T_PWD,
		T_UNSET, T_EXPORT, T_EXIT};

	if (!arg)
		return (T_ERROR);
	i = 0;
	type = T_OTHERS;
	while (i < 8 && *builtins[i])
	{
		if (!ft_strcmp(builtins[i], arg))
		{
			type = types[i];
			break ;
		}
		i++;
	}
	return (type);
}

static char	*search_path_builtin(t_env *env_list, char *command)
{
	int		i;
	char	*temp;
	char	**paths;
	t_env	*current;
	char	*pathname;

	current = env_list;
	while (current && current->name && ft_strcmp(current->name, "PATH"))
		current = current->next;
	if (!current || !current->value)
		paths = ft_split(DEFAULT_PATH_1 DEFAULT_PATH_2, ':');
	else
		paths = ft_split(current->value, ':');
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin("/", command);
		pathname = ft_strjoin(paths[i], temp);
		free(temp);
		if (!access(pathname, X_OK))
			return (free_double_ptr(paths), pathname);
		free(pathname);
	}
	free_double_ptr(paths);
	return (NULL);
}

static int	execute_builtin(t_master *master, t_exec *exec, t_builtin_type type)
{
	if (type == T_CD)
		return (ft_cd(exec->argc, exec->argv, master));
	else if (type == T_ECHO)
		return (ft_echo(exec->argc, exec->argv, master));
	else if (type == T_ENV)
		return (ft_env(master), T_ENV);
	else if (type == T_EXPORT)
		return (ft_export(exec->argc, exec->argv, master), T_EXPORT);
	else if (type == T_PWD)
		return (ft_pwd(), T_PWD);
	else if (type == T_UNSET)
		return (ft_unset(exec->argc, exec->argv, master), T_UNSET);
	else if (type == T_EXIT)
		ft_exit(master, exec->argc, exec->argv);
	return (T_ERROR);
}

static bool	execute_command(t_master *master, t_exec *exec)
{
	struct stat	s;

	exec->pathname = search_path_builtin(master->env_list, exec->argv[0]);
	if (!exec->pathname)
	{
		if (access(exec->argv[0], X_OK) == 0)
		{
			stat(exec->argv[0], &s);
			if (S_ISDIR(s.st_mode))
			{
				printf("minishell: %s: Is a directory\n", exec->argv[0]);
				master->exit_status = 126;
				return (false);
			}
			exec->pathname = ft_strdup(exec->argv[0]);
		}
		else
		{
			printf("minishell: %s: command not found\n", exec->argv[0]);
			master->exit_status = 127;
			return (false);
		}
	}
	return (true);
}

t_builtin_type	execute_command_or_builtin(t_master *master, t_exec *exec)
{
	t_builtin_type	type;

	type = inspect_token(exec->argv[0]);
	if (type == T_ERROR || !ft_strcmp(exec->argv[0], ".")
		|| !ft_strcmp(exec->argv[0], ".."))
	{
		handle_error_cases(master, exec);
		return (T_ERROR);
	}
	else if (type != T_OTHERS)
	{
		master->exit_status = execute_builtin(master, exec, type);
		return (type);
	}
	else if (!execute_command(master, exec))
		return (T_ERROR);
	return (T_OTHERS);
}
