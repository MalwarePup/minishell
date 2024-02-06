/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:33:30 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/06 19:30:16 by alfloren         ###   ########.fr       */
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
			error_exit(master, "malloc (env_list_to_array)"),
			NULL);
	i = 0;
	while (env_list)
	{
		tmp = ft_strjoin(env_list->name, "=");
		if (!tmp)
			error_exit(master, "Failed to allocate memory for tmp");
		array[i] = ft_strjoin(tmp, env_list->value);
		free(tmp);
		if (!array[i])
			error_exit(master, "Failed to allocate memory for array[i]");
		env_list = env_list->next;
		i++;
	}
	return (array[i] = NULL, array);
}

t_exec	*init(t_master *master, int *status, int *num_pids)
{
	t_exec	*new;

	new = ft_calloc(1, sizeof(t_exec));
	if (!new)
	{
		perror("ft_calloc in format_arg");
		cleanup_before_exit(master);
		exit(EXIT_FAILURE);
	}
	g_exit_status = 0;
	*num_pids = 0;
	*status = 0;
	return (new);
}

void	execute_command(t_master *master)
{
	char	**envp;

	envp = env_list_to_array(master, master->env_list);
	execve(master->exec->pathname, master->exec->argv, envp);
	free_double_ptr(envp);
	cleanup_executable(master);
	error_exit(master, "execve (execute_command)");
}

void	chose_execute(t_master *master, t_cmd_type type)
{
	if (master->exec->pathname)
		execute_command(master);
	else
		execute_builtin(master, type);
}
