/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:41:29 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/28 16:07:07 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"

char	*update_shlvl(t_master *master, char *value, char *name)
{
	int	new_value;

	new_value = ft_atoi(value);
	free(value);
	new_value++;
	value = ft_itoa(new_value);
	if (!value)
	{
		free(name);
		error_exit(master, "ft_itoa (update_shlvl)");
	}
	return (value);
}

void	update_executable_path(t_master *master, t_env *current)
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
			value_size = ft_strlen(master->argv[master->argc - 1]) + 1;
			if (value_size)
			{
				value = malloc(value_size);
				ft_strlcpy(value, master->argv[master->argc - 1], value_size);
				current->value = value;
			}
			return ;
		}
		current = current->next;
	}
}
