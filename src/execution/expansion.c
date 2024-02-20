/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:34:31 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/20 12:36:30 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

static char	*create_new_string_with_value(t_master *master, char *str,
	t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	exp->is_expanded = true;
	len = ft_strlen(str) - ft_strlen(exp->name) - 1 + ft_strlen(exp->value) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free(exp->name);
		free(exp->value);
		ft_error_exit(master, "malloc (create_new_string_with_value)", ENOMEM);
	}
	ft_strlcpy(new_str, str, exp->substr_start - str + 1);
	ft_strlcat(new_str, exp->value, len);
	ft_strlcat(new_str, exp->substr_start + ft_strlen(exp->name) + 1, len);
	return (new_str);
}

static char	*create_new_string_without_value(t_master *master, char *str,
	t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	exp->is_expanded = true;
	len = ft_strlen(str) - ft_strlen(exp->name) - 1 + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free(exp->name);
		ft_error_exit(master, "malloc (create_new_string_without_value)",
			ENOMEM);
	}
	if (len == 1)
		new_str[0] = '\0';
	else
	{
		ft_strlcpy(new_str, str, exp->substr_start - str + 1);
		ft_strlcat(new_str, exp->substr_start + ft_strlen(exp->name) + 1, len);
	}
	return (new_str);
}

static void	process_expansion_replace(t_master *master, char **str,
	t_expansion *exp)
{
	char	*new_str;

	if (exp->value)
		new_str = create_new_string_with_value(master, *str, exp);
	else
		new_str = create_new_string_without_value(master, *str, exp);
	free(*str);
	*str = new_str;
}

static void	process_expansion(t_master *master, char **str, t_expansion *exp)
{
	if ((*str)[exp->i + 1] == '\0' || ft_isspace((*str)[exp->i + 1]) ||
		((*str)[exp->i + 1] == '"' && exp->quote == '"'))
		return ;
	exp->substr_start = (*str) + exp->i;
	exp->name = extract_expansion_name(master, exp->substr_start);
	if (!is_valid_expansion_name(exp->name) && (*str)[exp->i + 1] != '?')
	{
		free(exp->name);
		return ;
	}
	if ((*str)[exp->i + 1] == '?')
		exp->value = ft_itoa(master->last_command_exit_value);
	else
		exp->value = getenv_value(master, master->env_list, exp->name);
	process_expansion_replace(master, str, exp);
}

void	launch_expansion(t_master *master, char **str)
{
	t_expansion	exp;

	init_expansion(&exp);
	while ((*str)[exp.i])
	{
		condition_while(*str, exp.i, true, &exp.quote);
		if ((*str)[exp.i] == '$' && exp.quote != '\'')
		{
			process_expansion(master, str, &exp);
			if (ft_strlen(*str) < 1)
				break ;
			else if (exp.is_expanded)
			{
				init_expansion(&exp);
				continue ;
			}
		}
		exp.i++;
	}
}
