/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:50:01 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/10 23:03:03 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static void	remove_var(t_master *master, t_env_list *env)
{
	t_env_list	*current;
	t_env_list	*previous;

	current = master->env;
	previous = NULL;
	while (current && ft_strcmp(current->name, env->name) != 0)
	{
		previous = current;
		current = current->next;
	}
	if (current)
	{
		if (previous)
		{
			previous->next = current->next;
			if (!current->next)
				master->env->last = previous;
		}
		else
			master->env = current->next;
		free(current->name);
		free(current->value);
		free(current);
	}
}

int	builtin_unset(char **argv, t_master *master)
{
	int			i;
	t_env_list	*current;
	t_env_list	*next;

	i = 1;
	while (argv[i])
	{
		if (ft_strcmp(argv[i], "PATH") == 0)
			master->path_unset = true;
		current = master->env;
		while (current)
		{
			next = current->next;
			if (ft_strcmp(argv[i], current->name) == 0)
			{
				if (ft_strcmp(current->name, "PATH") == 0)
					master->path_unset = true;
				remove_var(master, current);
			}
			current = next;
		}
		i++;
	}
	return (0);
}
