/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:34:31 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 21:18:03 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

static char	*create_new_string_with_value(t_master *master, t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(master->argv[(*exp).i]) + ft_strlen((*exp).value) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		free((*exp).value);
		ft_error_exit(master, "malloc (create_new_string_with_value)", ENOMEM);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, master->argv[(*exp).i],
		(*exp).substr_start - master->argv[(*exp).i] + 1);
	ft_strlcat(new_str, (*exp).value, len);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static char	*create_new_string_without_value(t_master *master, t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(master->argv[(*exp).i]) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		ft_error_exit(master, "malloc (create_new_string_with_value)", ENOMEM);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, master->argv[(*exp).i],
		(*exp).substr_start - master->argv[(*exp).i] + 1);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static void	process_expansion_replace(t_master *master, t_expansion *exp)
{
	char	*new_str;

	if ((*exp).value)
		new_str = create_new_string_with_value(master, exp);
	else
		new_str = create_new_string_without_value(master, exp);
	free(master->argv[(*exp).i]);
	master->argv[(*exp).i] = new_str;
}

static void	process_expansion(t_master *master, t_expansion *exp)
{
	exp->substr_start = master->argv[exp->i] + exp->j;
	if (master->argv[(*exp).i][(*exp).j + 1] == '?')
	{
		(*exp).name = extract_expansion_name(master, (*exp).substr_start);
		(*exp).value = ft_itoa(master->prev_exit_status);
		if (!(*exp).value)
		{
			free((*exp).name);
			ft_error_exit(master, "ft_itoa (process_expansion)", ENOMEM);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		(*exp).name = extract_expansion_name(master, (*exp).substr_start);
		(*exp).value = get_env_value(master, master->env_list, (*exp).name);
	}
	process_expansion_replace(master, exp);
	free((*exp).name);
	free((*exp).value);
}

void	launch_expansion(t_master *master)
{
	t_expansion	exp;
	char		quote;

	exp.i = 0;
	quote = 0;
	if (!master->exec || !master->argv)
		return ;
	while (master->argc > 0 && master->argv[exp.i])
	{
		exp.j = 0;
		while (master->argv[exp.i][exp.j])
		{
			condition_while(master->argv[exp.i], exp.j, true, &quote);
			if (master->argv[exp.i][exp.j] == '$' && quote != '\''
				&& (ft_isalpha(master->argv[exp.i][exp.j + 1])
				|| master->argv[exp.i][exp.j + 1] == '?') && (exp.j == 0
				|| (exp.j > 0 && master->argv[exp.i][exp.j - 1] != '$')))
				process_expansion(master, &exp);
			else
				exp.j++;
		}
		replace_argv_without_quotes(master, &exp);
		exp.i++;
	}
}
