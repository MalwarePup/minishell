/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:09 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/13 11:55:32 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

bool	pathname_is_findable(t_master *master, char **pathname, char **paths)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin("/", master->exec->argv[0]);
		if (!temp)
			ft_error_exit(master, "ft_strjoin (pathname_is_findable)",
				ENOMEM, true);
		*pathname = ft_strjoin3(paths[i], temp);
		if (!*pathname)
			ft_error_exit(master, "ft_strjoin (pathname_is_findable)",
				ENOMEM, true);
		if (access(*pathname, X_OK) == 0)
			return (free_string_array(paths), true);
		else if (errno == EACCES)
			return (free_string_array(paths), true);
		free(*pathname);
		i++;
	}
	return (free_string_array(paths), false);
}

static char	*find_executable_command_path(t_master *master)
{
	char	**paths;
	t_env	*current;
	char	*pathname;

	if (special_cases(master, &pathname))
		return (pathname);
	current = master->env_list;
	while (current && current->name && ft_strcmp(current->name, "PATH"))
		current = current->next;
	if (!current || !current->value)
		paths = ft_split(DEFAULT_PATH_1 DEFAULT_PATH_2, ':');
	else
		paths = ft_split(current->value, ':');
	if (pathname_is_findable(master, &pathname, paths))
		return (pathname);
	return (NULL);
}

static bool	is_executable_command(t_master *master)
{
	if (ft_strcmp(master->exec->argv[0], "..")
		&& ft_strcmp(master->exec->argv[0], ".")
		&& ft_strcmp(master->exec->argv[0], "./"))
	{
		master->exec->pathname = find_executable_command_path(master);
		if (errno == EACCES)
		{
			ft_dprintf(STDERR_FILENO, ESTR_PERM_DENIED, master->exec->pathname);
			master->exit_status = EXIT_CANNOT_EXECUTE;
			return (false);
		}
	}
	if (!master->exec->pathname)
	{
		handle_command_not_found_error(master);
		return (false);
	}
	return (true);
}

static t_cmd_type	identify_builtin_command(char *arg)
{
	size_t			i;
	t_cmd_type		type;
	size_t			num_builtins;
	const t_builtin	builtins[] = {{"cd", CMD_CD}, {"echo", CMD_ECHO}, {"env",
		CMD_ENV}, {"exit", CMD_EXIT}, {"pwd", CMD_PWD}, {"unset", CMD_UNSET},
	{"export", CMD_EXPORT}, {"exit", CMD_EXIT}};

	type = CMD_OTHERS;
	num_builtins = sizeof(builtins) / sizeof(t_builtin);
	if (!arg)
		return (CMD_ERROR);
	i = -1;
	while (++i < num_builtins)
	{
		if (!ft_strcmp(builtins[i].name, arg))
		{
			type = builtins[i].type;
			break ;
		}
	}
	return (type);
}

t_cmd_type	execute_command_or_builtin(t_master *master)
{
	t_cmd_type	type;

	type = identify_builtin_command(master->exec->argv[0]);
	if (type == CMD_ERROR)
		return (CMD_ERROR);
	else if (type != CMD_OTHERS)
		return (type);
	else if (!is_executable_command(master))
		return (CMD_ERROR);
	return (CMD_OTHERS);
}
