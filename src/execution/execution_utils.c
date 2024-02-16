/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:33:30 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 10:20:55 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static size_t	get_env_list_size(t_env *env_list)
{
	size_t	size;

	size = 0;
	while (env_list)
	{
		size++;
		env_list = env_list->next;
	}
	return (size);
}

static void	handle_error(t_master *master, char **array)
{
	free_string_array(array);
	ft_error_exit(master, "ft_strjoin (env_list_to_array)", ENOMEM, true);
}

char	**env_list_to_array(t_master *master, t_env *env_list)
{
	size_t	i;
	char	*tmp;
	char	**array;

	if (!env_list)
		return (NULL);
	array = malloc((get_env_list_size(env_list) + 1) * sizeof(char *));
	if (!array)
		error_exit(master, "malloc (env_list_to_array)", true);
	i = 0;
	while (env_list)
	{
		tmp = ft_strjoin(env_list->name, "=");
		if (!tmp)
			handle_error(master, array);
		array[i] = ft_strjoin1(tmp, env_list->value);
		if (!array[i])
			handle_error(master, array);
		env_list = env_list->next;
		i++;
	}
	return (array[i] = NULL, array);
}

void	init_exec(t_master *master)
{
	pid_t	*pid;

	pid = malloc(sizeof(pid_t) * count_pipe(master->token));
	if (!pid)
		ft_error_exit(master, "malloc (init_exec)", ENOMEM, false);
	master->pids = pid;
	master->exec = ft_calloc(1, sizeof(t_exec));
	if (!master->exec)
		ft_error_exit(master, "ft_calloc (ft_calloc)", ENOMEM, false);
	master->exec->pid = -1;
	master->exec->pipefd[0] = -1;
	master->exec->pipefd[1] = -1;
	master->exec->first_cmd = true;
}

void	execute_command(t_master *master)
{
	char	**envp;

	envp = env_list_to_array(master, master->env_list);
	execve(master->exec->argv[0], master->exec->argv, envp);
	free_string_array(envp);
	error_exit(master, "execve (execute_command)", true);
}
