/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:09 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/13 11:11:50 by alfloren         ###   ########.fr       */
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

static char	*find_executable_command_path(t_master *master)
{
	int		i;
	char	*temp;
	char	**paths;
	t_env	*current;
	char	*pathname;

	if (master->exec->argv[0][0] == '\0')
		return (NULL);
	if (access(master->exec->argv[0], X_OK) == 0)
	{
		pathname = ft_strdup(master->exec->argv[0]);
		if (!pathname)
			ft_error_exit(master, "ft_strdup (find_executable_command_path)",
				ENOMEM, true);
		return (pathname);
	}
	current = master->env_list;
	while (current && current->name && ft_strcmp(current->name, "PATH"))
		current = current->next;
	if (!current || !current->value)
		paths = ft_split(DEFAULT_PATH_1 DEFAULT_PATH_2, ':');
	else
		paths = ft_split(current->value, ':');
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin("/", master->exec->argv[0]);
		pathname = ft_strjoin3(paths[i], temp);
		if (access(pathname, X_OK) == 0)
			return (free_string_array(paths), pathname);
		else if (errno == EACCES)
			return (free_string_array(paths), pathname);
		free(pathname);
	}
	return (free_string_array(paths), NULL);
}

static void	handle_command_not_found_error(t_master *master)
{
	struct stat	s;

	if (access(master->exec->argv[0], X_OK) == 0
		&& ft_strcmp(master->exec->argv[0], ".."))
	{
		stat(master->exec->argv[0], &s);
		if (S_ISDIR(s.st_mode) && ft_strcmp(master->exec->argv[0], "."))
		{
			ft_dprintf(STDERR_FILENO, ESTR_DIR, master->exec->argv[0]);
			master->exit_status = EXIT_CANNOT_EXECUTE;
		}
		else
		{
			ft_dprintf(STDERR_FILENO, ESTR_DOT_P1 ESTR_DOT_P2);
			master->exit_status = EXIT_MISUSE;
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, ESTR_CMD_NOT_FOUND, master->exec->argv[0]);
		master->exit_status = EXIT_NOT_FOUND;
	}
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

int	execute_builtin(t_master *master, t_cmd_type type)
{
	if (type == CMD_CD)
		return (ft_cd(master->exec->argc, master->exec->argv, master));
	else if (type == CMD_ECHO)
		return (ft_echo(master->exec->argc, master->exec->argv, master));
	else if (type == CMD_ENV)
		return (ft_env(master), CMD_ENV);
	else if (type == CMD_EXPORT)
		return (ft_export(master->exec->argc,
				master->exec->argv, master), CMD_EXPORT);
	else if (type == CMD_PWD)
		return (ft_pwd(), CMD_PWD);
	else if (type == CMD_UNSET)
		return (ft_unset(master->exec->argc,
				master->exec->argv, master), CMD_UNSET);
	else if (type == CMD_EXIT)
		ft_exit(master, master->exec->argc, master->exec->argv);
	return (CMD_ERROR);
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
