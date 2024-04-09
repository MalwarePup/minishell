/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:34:31 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/09 07:10:14 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

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
		error_exit(master, "malloc (create_new_string_without_value)");
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
	if ((*str)[exp->i + 1] == '?')
		exp->value = ft_itoa(master->last_command_exit_value);
	else
		exp->value = getenv_value(master, master->env, exp->name);
	process_expansion_replace(master, str, exp);
	free(exp->name);
	free(exp->value);
}

static int	handle_expansion_iteration(t_master *master, char **str,
	t_expansion *exp)
{
	is_valid_character((*str)[exp->i], true, &exp->quote);
	if ((*str)[exp->i] == '$' && exp->quote != '\''
		&& (ft_isalnum((*str)[exp->i + 1]) || (*str)[exp->i + 1] == '_'
		|| (*str)[exp->i + 1] == '?' || (*str)[exp->i + 1] == '\''
		|| (*str)[exp->i + 1] == '"'))
	{
		process_expansion(master, str, exp);
		if (ft_strlen(*str) < 1)
			return (1);
		else if (exp->is_expanded)
			ft_memset(exp, 0, sizeof(t_expansion));
	}
	else if (ft_strncmp(&(*str)[exp->i], "<<", 2) == 0
		&& !exp->heredoc_detected)
	{
		exp->heredoc_detected = true;
		exp->i += 2;
		while (ft_isspace((*str)[exp->i]))
			exp->i++;
		while ((*str)[exp->i] && !ft_isspace((*str)[exp->i]))
			exp->i++;
		exp->heredoc_detected = false;
		return (0);
	}
	return (0);
}

void	launch_expansion(t_master *master, char **str)
{
	t_expansion	exp;

	ft_memset(&exp, 0, sizeof(t_expansion));
	while ((*str)[exp.i])
	{
		if (handle_expansion_iteration(master, str, &exp))
			break ;
		exp.i++;
	}
}
