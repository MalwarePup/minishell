/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:43 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/13 13:34:25 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_dprintf.h"
#include <unistd.h>
#include <sys/stat.h>
#include "libft.h"
#include <errno.h>

void	handle_command_not_found_error(t_master *master)
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

int	execute_builtin(t_master *master, t_cmd_type type)
{
	if (type == CMD_CD)
		return (ft_cd(master->exec->argc, master->exec->argv, master));
	else if (type == CMD_ECHO)
		return (ft_echo(master->exec->argc, master->exec->argv, master));
	else if (type == CMD_ENV)
		return (ft_env(master));
	else if (type == CMD_EXPORT)
		return (ft_export(master->exec->argc, master->exec->argv, master));
	else if (type == CMD_PWD)
		return (ft_pwd());
	else if (type == CMD_UNSET)
		return (ft_unset(master->exec->argc, master->exec->argv, master));
	else if (type == CMD_EXIT)
		ft_exit(master, master->exec->argc, master->exec->argv);
	return (CMD_ERROR);
}

bool	special_cases(t_master *master, char **pathname)
{
	if (master->exec->argv[0][0] == '\0')
	{
		*pathname = NULL;
		return (true);
	}
	if (access(master->exec->argv[0], X_OK) == 0)
	{
		*pathname = ft_strdup(master->exec->argv[0]);
		if (!*pathname)
			ft_error_exit(master, "ft_strdup (find_executable_command_path)",
				ENOMEM, true);
		return (true);
	}
	return (false);
}
