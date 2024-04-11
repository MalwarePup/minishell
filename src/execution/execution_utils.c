/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 20:33:30 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/11 15:56:58 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static size_t	get_env_list_size(t_env_list *env)
{
	size_t	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

void	update_executable_path(t_master *master, t_env_list *current)
{
	char	*value;
	size_t	value_size;

	if (!master->argv || !master->argv[0])
		return ;
	value = NULL;
	while (current && current->name)
	{
		if (!ft_strcmp(current->name, "_"))
		{
			free(current->value);
			current->value = NULL;
			value_size = ft_strlen(master->argv[master->argc - 1]) + 1;
			if (value_size)
			{
				value = malloc(value_size);
				if (!value)
					error_exit(master, "malloc (update_executable_path)");
				ft_strlcpy(value, master->argv[master->argc - 1], value_size);
				current->value = value;
			}
			return ;
		}
		current = current->next;
	}
}

char	**env_list_to_array(t_master *master, t_env_list *env)
{
	size_t	i;
	char	*tmp;
	char	**array;

	if (!env)
		return (NULL);
	array = malloc((get_env_list_size(env) + 1) * sizeof(char *));
	if (!array)
		error_exit(master, "malloc (env_list_to_array)");
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->name, "=");
		array[i] = ft_strjoin1(tmp, env->value);
		if (!tmp || !array[i])
		{
			free(tmp);
			free_string_array(&array);
			error_exit(master, "ft_strjoin (env_list_to_array)");
		}
		env = env->next;
		i++;
	}
	return (array[i] = NULL, array);
}

void	init_exec(t_master *master)
{
	master->pid_list = malloc(count_pipe(master->token) * sizeof(pid_t));
	if (!master->pid_list)
		error_exit(master, "malloc (init_exec)");
	master->exec = ft_calloc(1, sizeof(t_exec));
	if (!master->exec)
		error_exit(master, "ft_calloc (init_exec)");
	master->exec->pid = -1;
	master->exec->pipefd[0] = -1;
	master->exec->pipefd[1] = -1;
	master->exec->first_cmd = true;
}

void	execute_command(t_master *master)
{
	char	**envp;

	if (!is_executable_command(master))
		return ;
	envp = env_list_to_array(master, master->env);
	execve(master->argv[0], master->argv, envp);
	free_string_array(&envp);
	error_exit(master, "execve (execute_command)");
}
