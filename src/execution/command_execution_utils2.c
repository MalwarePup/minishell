/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:42:44 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/13 16:36:22 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include "libft.h"
#include <errno.h>
#include "minishell.h"
#include "ft_dprintf.h"

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

	if (!ft_strcmp(master->argv[0], ".."))
	{
		ft_dprintf(STDERR_FILENO, ESTR_CMD_NOT_FOUND, master->argv[0]);
		return (master->exit_status = NOT_FOUND, false);
	}
	else if (stat(master->argv[0], &s) == 0)
	{
		if ((S_ISDIR(s.st_mode) && ft_strcmp(master->argv[0], "."))
			&& (!ft_strncmp(master->argv[0], "./", 2)
				|| !ft_strncmp(master->argv[0], "/", 1)
				|| master->argv[0][ft_strlen(master->argv[0]) - 1] == '/'))
		{
			ft_dprintf(STDERR_FILENO, ESTR_DIR, master->argv[0]);
			return (master->exit_status = CANNOT_EXECUTE, false);
		}
	}
	else
		error_exit(master, "stat (handle_directory_access_error)");
	return (true);
}

static bool	handle_file_access_and_errors(t_master *master)
{
	if (access(master->argv[0], X_OK) == 0)
		return (handle_directory_access_error(master));
	else if (errno == EACCES)
	{
		ft_dprintf(STDERR_FILENO, ESTR_PERM_DENIED, master->argv[0]);
		return (master->exit_status = CANNOT_EXECUTE, false);
	}
	else if (errno == ENOENT
		&& (!ft_strncmp(master->argv[0], "./", 2)
			|| !ft_strncmp(master->argv[0], "/", 1)))
	{
		if (!master->token || !master->token->redir)
		{
			ft_dprintf(STDERR_FILENO, ESTR_NO_FILE, master->argv[0]);
			return (master->exit_status = NOT_FOUND, false);
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, ESTR_CMD_NOT_FOUND, master->argv[0]);
		return (master->exit_status = NOT_FOUND, false);
	}
	return (true);
}

bool	handle_command_not_found_error(t_master *master)
{
	if (!check_file_executability(master->argv[0])
		&& ft_strncmp(master->argv[0], "/", 1)
		&& ft_strncmp(master->argv[0], "./", 2)
		&& master->argv[0][ft_strlen(master->argv[0]) - 1] != '/')
	{
		if (ft_strcmp(master->argv[0], ".") == 0
			&& master->argv[0][1] == '\0')
		{
			ft_dprintf(STDERR_FILENO, ESTR_DOT_P1 ESTR_DOT_P2);
			return (master->exit_status = MISUSE, false);
		}
	}
	return (handle_file_access_and_errors(master));
}
