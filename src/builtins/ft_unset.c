/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:50:01 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/28 11:31:49 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static void	remove_var(t_master *master, t_env *env)
{
	t_env	*current;
	t_env	*previous;

	current = master->env_list;
	previous = NULL;
	while (current && ft_strcmp(current->name, env->name) != 0)
	{
		previous = current;
		current = current->next;
	}
	if (current)
	{
		if (previous)
			previous->next = current->next;
		else
			master->env_list = current->next;
		free(current->name);
		free(current->value);
		free(current);
	}
}

int	ft_unset(char **argv, t_master *master)
{
	t_env	*current;

	current = master->env_list;
	while (current && argv[1])
	{
		if (ft_strcmp(argv[1], current->name) == 0)
		{
			remove_var(master, current);
			break ;
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
