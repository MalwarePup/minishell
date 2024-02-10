/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:33:30 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/10 12:37:20 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	**env_list_to_array(t_master *master, t_env *env_list)
{
	size_t	i;
	char	*tmp;
	char	**array;

	if (!env_list)
		return (NULL);
	array = malloc((get_env_list_size(env_list) + 1) * sizeof(char *));
	if (!array)
		return (cleanup_executable(master),
			error_exit(master, "malloc (env_list_to_array)", false),
			NULL);
	i = 0;
	while (env_list)
	{
		tmp = ft_strjoin(env_list->name, "=");
		if (!tmp)
			error_exit(master, "Failed to allocate memory for tmp", false);
		array[i] = ft_strjoin(tmp, env_list->value);
		free(tmp);
		if (!array[i])
			error_exit(master, "Failed to allocate memory for array[i]", false);
		env_list = env_list->next;
		i++;
	}
	return (array[i] = NULL, array);
}

void	init_exec(t_master *master, int *status, int *num_pids)
{
	master->exec = ft_calloc(1, sizeof(t_exec));
	if (!master->exec)
	{
		perror("ft_calloc in format_arg");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	*status = 0;
	*num_pids = 0;
	g_exit_status = 0;
	master->exec->pid = -1;
	master->exec->pipefd[0] = -1;
	master->exec->pipefd[1] = -1;
	master->exec->first_cmd = true;
}

void	execute_command(t_master *master)
{
	char	**envp;

	envp = env_list_to_array(master, master->env_list);
	execve(master->exec->pathname, master->exec->argv, envp);
	free_double_ptr(envp);
	// cleanup_executable(master);
	error_exit(master, "execve (execute_command)", true);
}

void	chose_execute(t_master *master, t_cmd_type type)
{
	if (master->exec->pathname)
		execute_command(master);
	else
		execute_builtin(master, type);
}
