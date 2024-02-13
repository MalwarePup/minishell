/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:06:23 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/13 13:33:30 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"

int	ft_env(t_master *master)
{
	t_env	*current;

	current = master->env_list;
	while (current)
	{
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
