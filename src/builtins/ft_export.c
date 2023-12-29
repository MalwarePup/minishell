/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:00:36 by ladloff           #+#    #+#             */
/*   Updated: 2023/06/20 19:49:52 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "minishell.h"
#include "libft.h"

static bool	is_valid_variable_name(char	*name, char *var_str)
{
	if (!ft_isalpha(name[0]) && name[0] != '_')
	{
		printf("minishell: export: '%s': not a valid identifier\n", var_str);
		return (false);
	}
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
		{
			printf("minishell: export: '%s': not a valid identifier\n", var_str);
			return (false);
		}
		name++;
	}
	return (true);
}

static bool	is_var_already_in_env(t_master **master, t_env *var)
{
	t_env	*current;

	current = (*master)->env_list;
	if (!current->name || !var->name)
	{
		return (false);
	}
	while (current)
	{
		if (!ft_strcmp(current->name, var->name))
		{
			free(current->value);
			current->value = var->value;
			free(var->name);
			return (true);
		}
		current = current->next;
	}
	return (false);
}

static void	add_back_env_var(t_master **master, t_env *var)
{
	if (!(*master)->env_list)
		(*master)->env_list = var;
	else
	{
		(*master)->env_list->last->next = var;
		(*master)->env_list->last = var;
	}
}

static int	export_var(t_master *master, char *var_str, char *equals_location)
{
	t_env	*var;
	bool	status;

	var = malloc(1 * sizeof(t_env));
	if (!var)
		return (EXIT_FAILURE);
	if (equals_location)
	{
		*equals_location = '\0';
		var->name = ft_strdup(var_str);
		var->value = ft_strdup(equals_location + 1);
	}
	else
	{
		var->name = ft_strdup(var_str);
		var->value = ft_strdup("");
	}
	status = is_var_already_in_env(&master, var);
	if (status)
		return (free(var), EXIT_SUCCESS);
	var->next = NULL;
	if (!is_valid_variable_name(var->name, var_str))
		return (free(var->name), free(var->value), free(var), EXIT_FAILURE);
	add_back_env_var(&master, var);
	return (EXIT_SUCCESS);
}

int	ft_export(int argc, char **argv, t_master *master)
{
	int		i;
	t_env	*current;
	char	*equals_location;

	i = 0;
	current = master->env_list;
	if (argc < 2)
	{
		while (current)
		{
			printf("export %s=\"%s\"\n", current->name, current->value);
			current = current->next;
		}
		return (EXIT_SUCCESS);
	}
	while (++i < argc)
	{
		equals_location = ft_strchr(argv[i], '=');
		if (export_var(master, argv[i], equals_location))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
