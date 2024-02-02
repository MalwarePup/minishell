/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:09 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/02 14:49:46 by alfloren         ###   ########.fr       */
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

static char	*search_path_command(t_env *env_list, char *command)
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

static bool	is_executable_command(t_master *master, t_exec *exec)
{
	struct stat	s;

	exec->pathname = search_path_command(master->env_list, exec->argv[0]);
	if (!exec->pathname)
	{
		if (access(exec->argv[0], X_OK) == 0)
		{
			stat(exec->argv[0], &s);
			if (S_ISDIR(s.st_mode))
			{
				ft_dprintf(STDERR_FILENO, ESTR_DIR, exec->argv[0]);
				g_exit_status = EXIT_CANNOT_EXECUTE;
				return (false);
			}
			exec->pathname = ft_strdup(exec->argv[0]);
		}
		else
		{
			handle_command_error(exec);
			return (false);
		}
	}
	return (true);
}

static t_cmd_type	inspect_token(char *arg)
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

int	execute_builtin(t_master *master, t_exec *exec, t_cmd_type type)
{
	if (type == CMD_CD)
		return (ft_cd(exec->argc, exec->argv, master));
	else if (type == CMD_ECHO)
		return (ft_echo(exec->argc, exec->argv, master));
	else if (type == CMD_ENV)
		return (ft_env(master), CMD_ENV);
	else if (type == CMD_EXPORT)
		return (ft_export(exec->argc, exec->argv, master), CMD_EXPORT);
	else if (type == CMD_PWD)
		return (ft_pwd(), CMD_PWD);
	else if (type == CMD_UNSET)
		return (ft_unset(exec->argc, exec->argv, master), CMD_UNSET);
	else if (type == CMD_EXIT)
		ft_exit(master, exec->argc, exec->argv);
	return (CMD_ERROR);
}

t_cmd_type	execute_command_or_builtin(t_master *master, t_exec *exec)
{
	t_cmd_type	type;

	type = inspect_token(exec->argv[0]);
	if (type == CMD_ERROR || !ft_strcmp(exec->argv[0], ".")
		|| !ft_strcmp(exec->argv[0], ".."))
		return (handle_error_cases(exec), CMD_ERROR);
	else if (type != CMD_OTHERS)
		return (type);
	else if (!is_executable_command(master, exec))
		return (CMD_ERROR);
	return (CMD_OTHERS);
}
