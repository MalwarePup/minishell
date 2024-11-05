/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:09 by ladloff           #+#    #+#             */
/*   Updated: 2024/11/05 18:49:06 by ladloff          ###   ########.fr       */
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

bool	is_executable_command(t_master *master)
{
	if (ft_strcmp(master->argv[0], "..")
		&& ft_strcmp(master->argv[0], ".")
		&& ft_strcmp(master->argv[0], "./"))
	{
		if (master->argv[0][0] == '\0')
		{
			free(master->argv[0]);
			master->argv[0] = ft_strdup("");
			if (!master->argv[0])
				error_exit(master, "ft_strdup (is_executable_command)");
		}
		else
			find_executable_command_path(master);
	}
	if (handle_command_not_found_error(master) == false)
		return (false);
	return (true);
}

t_cmd_type	identify_builtin_command(char *arg)
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
	i = 0;
	while (i < num_builtins)
	{
		if (!ft_strcmp(builtins[i].name, arg))
		{
			type = builtins[i].type;
			break ;
		}
		i++;
	}
	return (type);
}

int	execute_builtin(t_master *master, t_cmd_type type)
{
	if (type == CMD_CD)
		return (builtin_cd((int)master->argc, master->argv, master));
	else if (type == CMD_ECHO)
		return (builtin_echo((int)master->argc, master->argv, master));
	else if (type == CMD_ENV)
		return (builtin_env(master));
	else if (type == CMD_EXPORT)
		return (builtin_export((int)master->argc, master->argv, master));
	else if (type == CMD_PWD)
		return (builtin_pwd());
	else if (type == CMD_UNSET)
		return (builtin_unset(master->argv, master));
	else if (type == CMD_EXIT)
		return (builtin_exit(master, (int)master->argc, master->argv));
	return (CMD_ERROR);
}
