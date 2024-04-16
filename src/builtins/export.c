/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:00:36 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/16 15:20:54 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"

static bool	is_valid_variable_name(char	*name, char *var_str)
{
	if (!ft_isalpha(name[0]) && name[0] != '_')
	{
		ft_dprintf(STDERR_FILENO, ESTR_INVALID_ID, var_str);
		return (false);
	}
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
		{
			ft_dprintf(STDERR_FILENO, ESTR_INVALID_ID, var_str);
			return (false);
		}
		name++;
	}
	return (true);
}

static bool	is_var_already_in_env(t_master **master, t_env_list *var)
{
	t_env_list	*current;

	current = (*master)->env;
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

static void	add_back_env_var(t_master **master, t_env_list *var)
{
	if (!(*master)->env)
	{
		(*master)->env = var;
		(*master)->env->last = var;
	}
	else
	{
		if ((*master)->env->last)
		{
			(*master)->env->last->next = var;
			(*master)->env->last = var;
		}
		else
		{
			(*master)->env->last = var;
		}
	}
}

static int	export_var(t_master *master, char *var_str, char *equals_location)
{
	t_env_list	*var;
	bool		status;

	var = malloc(1 * sizeof(t_env_list));
	if (!var)
		return (1);
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
		return (free(var), 0);
	var->next = NULL;
	if (!is_valid_variable_name(var->name, var_str))
		return (free(var->name), free(var->value), free(var), 1);
	add_back_env_var(&master, var);
	return (0);
}

int	builtin_export(int argc, char **argv, t_master *master)
{
	int			i;
	t_env_list	*current;
	char		*equals_location;

	i = 0;
	current = master->env;
	if (argc < 2)
	{
		while (current)
		{
			printf("export %s=\"%s\"\n", current->name, current->value);
			current = current->next;
		}
		return (0);
	}
	while (++i < argc)
	{
		equals_location = ft_strchr(argv[i], '=');
		if (export_var(master, argv[i], equals_location))
			return (1);
	}
	return (0);
}
