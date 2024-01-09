/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:34:31 by chmadran          #+#    #+#             */
/*   Updated: 2024/01/09 22:55:00 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "execution.h"
#include "libft.h"
#include "exit.h"

typedef struct s_expansion
{
	size_t	i;
	size_t	j;
	char	*name;
	char	*value;
	char	*substr_start;
}	t_expansion;

static char	*create_new_string_with_value(t_exec *exec, t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(exec->argv[(*exp).i]) + ft_strlen((*exp).value) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		free((*exp).value);
		cleanup_executable();
		ft_error_exit("malloc (create_new_string_with_value)", ENOMEM);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, exec->argv[(*exp).i],
		(*exp).substr_start - exec->argv[(*exp).i] + 1);
	ft_strlcat(new_str, (*exp).value, len);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static char	*create_new_string_without_value(t_exec *exec, t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(exec->argv[(*exp).i]) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		cleanup_executable();
		ft_error_exit("malloc (create_new_string_with_value)", ENOMEM);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, exec->argv[(*exp).i],
		(*exp).substr_start - exec->argv[(*exp).i] + 1);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static void	process_expansion_replace(t_exec *exec, t_expansion *exp)
{
	char	*new_str;

	if ((*exp).value)
		new_str = create_new_string_with_value(exec, exp);
	else
		new_str = create_new_string_without_value(exec, exp);
	free(exec->argv[(*exp).i]);
	exec->argv[(*exp).i] = new_str;
}

static void	process_expansion(t_master *master, t_exec *exec, t_expansion *exp)
{
	if (exec->argv[(*exp).i][(*exp).j + 1] == '?')
	{
		(*exp).name = extract_expansion_name((*exp).substr_start);
		(*exp).value = ft_itoa(master->exit_status);
		if (!(*exp).value)
		{
			free((*exp).name);
			cleanup_executable();
			ft_error_exit("ft_itoa (process_expansion)", ENOMEM);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		(*exp).name = extract_expansion_name((*exp).substr_start);
		(*exp).value = get_env_value(master->env_list, (*exp).name);
	}
	process_expansion_replace(exec, exp);
	free((*exp).name);
	free((*exp).value);
}

void	launch_expansion(t_master *master, t_exec *exec)
{
	t_expansion	exp;

	exp.i = 1;
	if (!exec || !exec->argv)
		return ;
	while (exec->argc > 1 && exec->argv[exp.i])
	{
		if (exec->argv[exp.i][exp.j] == '$' && exec->double_quotes)
		{
			if (exec->argv[exp.i][exp.j] == '$')
			{
				exp.substr_start = exec->argv[exp.i] + exp.j;
				process_expansion(master, exec, &exp);
			}
			else
				exp.j++;
		}
		exp.i++;
	}
}
