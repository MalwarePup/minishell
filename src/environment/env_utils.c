/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:41:29 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/19 11:51:38 by ladloff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "env.h"
#include "libft.h"
#include "exit.h"

void	print_environement_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

void	print_environement_var(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current && ft_strcmp(current->name, name))
		current = current->next;
	if (!current)
		return ;
	printf("%s=%s\n", current->name, current->value);
}

char	*update_shlvl(char *value, char *name)
{
	int	new_value;

	new_value = ft_atoi(value);
	free(value);
	new_value++;
	value = ft_itoa(new_value);
	if (!value)
	{
		free(name);
		ft_error_exit("ft_itoa (update_shlvl)", ENOMEM);
	}
	return (value);
}

void	update_executable_path(t_exec *exec, t_env *current)
{
	char	*value;
	size_t	value_size;

	if (!exec->argv || !exec->argv[0])
		return ;
	value = NULL;
	while (current && current->name)
	{
		if (!ft_strcmp(current->name, "_"))
		{
			free(current->value);
			value_size = ft_strlen(exec->argv[exec->argc - 1]) + 1;
			if (value_size)
			{
				value = malloc(value_size);
				ft_strlcpy(value, exec->argv[exec->argc - 1], value_size);
				current->value = value;
			}
			return ;
		}
		current = current->next;
	}
}
