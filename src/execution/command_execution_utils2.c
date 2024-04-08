/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:42:44 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/08 10:57:05 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

static char	**get_paths(t_master *master)
{
	t_env_list	*current;
	char		**paths;

	current = master->env;
	while (current && current->name && ft_strcmp(current->name, "PATH"))
		current = current->next;
	if (!current || !current->value)
		paths = ft_split(DEFAULT_PATH_1 DEFAULT_PATH_2, ':');
	else
		paths = ft_split(current->value, ':');
	if (!paths)
		error_exit(master, "ft_split (get_paths)");
	return (paths);
}

static char	*check_paths_for_executable(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*pathname;

	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin("/", cmd);
		pathname = ft_strjoin(paths[i], temp);
		free(temp);
		if (access(pathname, X_OK) == 0)
			return (pathname);
		free(pathname);
	}
	return (NULL);
}

void	find_executable_command_path(t_master *master)
{
	char	**paths;
	char	*executable_path;

	paths = get_paths(master);
	if ((master->argv[0][0] == '.' && master->argv[0][1] == '/')
		|| master->argv[0][0] == '/')
	{
		if (access(master->argv[0], X_OK) == 0)
		{
			free_string_array(&paths);
			return ;
		}
		else
		{
			free_string_array(&paths);
			return ;
		}
	}
	executable_path = check_paths_for_executable(paths, master->argv[0]);
	free_string_array(&paths);
	if (executable_path)
	{
		free(master->argv[0]);
		master->argv[0] = executable_path;
	}
	return ;
}
