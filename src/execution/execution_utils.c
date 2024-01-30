/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:33:30 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/30 16:13:58 by ladloff          ###   ########.fr       */
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

void	init(t_exec *exec, int *status, int *num_pids)
{
	g_exit_status = 0;
	*num_pids = 0;
	*status = 0;
	exec->pid = -1;
	exec->first_cmd = true;
	exec->pipefd[0] = -1;
	exec->pipefd[1] = -1;
	exec->argc = 0;
}
