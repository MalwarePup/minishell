/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:43 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/15 11:35:56 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_dprintf.h"
#include <unistd.h>
#include <sys/stat.h>
#include "libft.h"
#include <errno.h>

static bool	check_file_executability(char *path)
{
	struct stat	s;

	if (stat(path, &s) == 0)
		if (S_ISREG(s.st_mode) && (s.st_mode & S_IXUSR))
			return (true);
	return (false);
}

static bool	handle_directory_access_error(t_master *master)
{
	struct stat	s;

	if (stat(master->exec->argv[0], &s) == 0)
	{
		if ((S_ISDIR(s.st_mode) && ft_strcmp(master->exec->argv[0], "."))
			&& (!ft_strncmp(master->exec->argv[0], "./", 2)
				|| !ft_strncmp(master->exec->argv[0], "/", 1)))
		{
			ft_dprintf(STDERR_FILENO, ESTR_DIR, master->exec->argv[0]);
			return (master->exit_status = EXIT_CANNOT_EXECUTE, false);
		}
	}
	else
		error_exit(master, "stat (handle_command_not_found_error)", true);
	return (true);
}

static bool	handle_file_access_and_errors(t_master *master)
{
	if (access(master->exec->argv[0], X_OK) == 0)
		return (handle_directory_access_error(master));
	else if (errno == EACCES)
	{
		ft_dprintf(STDERR_FILENO, ESTR_PERM_DENIED, master->exec->argv[0]);
		return (master->exit_status = EXIT_CANNOT_EXECUTE, false);
	}
	else if (errno == ENOENT
		&& (!ft_strncmp(master->exec->argv[0], "./", 2)
			|| !ft_strncmp(master->exec->argv[0], "/", 1)))
	{
		ft_dprintf(STDERR_FILENO, ESTR_NO_FILE, master->exec->argv[0]);
		return (master->exit_status = EXIT_NOT_FOUND, false);
	}
	else
	{
		ft_dprintf(STDERR_FILENO, ESTR_CMD_NOT_FOUND, master->exec->argv[0]);
		return (master->exit_status = EXIT_NOT_FOUND, false);
	}
}

bool	handle_command_not_found_error(t_master *master)
{
	if (!check_file_executability(master->exec->argv[0])
		&& ft_strncmp(master->exec->argv[0], "/", 1)
		&& ft_strncmp(master->exec->argv[0], "./", 2))
	{
		if (ft_strcmp(master->exec->argv[0], ".") == 0
			&& master->exec->argv[0][1] == '\0')
		{
			ft_dprintf(STDERR_FILENO, ESTR_DOT_P1 ESTR_DOT_P2);
			return (master->exit_status = EXIT_MISUSE, false);
		}
		ft_dprintf(STDERR_FILENO, ESTR_CMD_NOT_FOUND, master->exec->argv[0]);
		return (master->exit_status = EXIT_NOT_FOUND, false);
	}
	return (handle_file_access_and_errors(master));
}
