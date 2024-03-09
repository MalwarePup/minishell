/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:50:01 by ladloff           #+#    #+#             */
/*   Updated: 2024/03/10 00:17:57 by ladloff          ###   ########.fr       */
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
			previous->next = current->next;
		else
			master->env = current->next;
		free(current->name);
		free(current->value);
		free(current);
	}
}

int	ft_unset(char **argv, t_master *master)
{
	t_env_list	*current;

	current = master->env;
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
