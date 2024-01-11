/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/20 11:38:23 by ladloff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

static bool	is_in_quotes(const char *line_read, size_t j)
{
	size_t	i;
	bool	inside_single_quotes;
	bool	inside_double_quotes;

	i = 0;
	inside_double_quotes = false;
	inside_single_quotes = false;
	while (line_read[i] && i < j)
	{
		if (line_read[i] == '\'' && line_read[i - 1] != '\\'
			&& !inside_double_quotes)
			inside_single_quotes = !inside_single_quotes;
		else if (line_read[i] == '\"' && line_read[i - 1] != '\\'
			&& !inside_single_quotes)
			inside_double_quotes = !inside_double_quotes;
		i++;
	}
	return (inside_single_quotes || inside_double_quotes);
}

static t_token_type	check_token_type(char c, const char *line_read, size_t *j)
{
	size_t				i;
	t_token_type		token_type;
	const char			*ops[OP] = {"|", "<", "<<", ">", ">>"};
	const t_token_type	types[OP] = {T_PIPE, T_RED_IN, T_D_RED_IN, T_RED_OUT,
		T_D_RED_OUT};

	i = 0;
	token_type = T_BUILTIN;
	while (i < OP)
	{
		if (is_in_quotes(line_read, *j))
			return (T_BUILTIN);
		if (c != *ops[i])
		{
			i++;
			continue ;
		}
		token_type = types[i];
		if (line_read[*j + 1] == c)
			token_type = types[i + 1];
		(*j)++;
		break ;
	}
	return (token_type);
}

static char	*trim_spaces(t_master *master, const char *str, size_t start,
	size_t end)
{
	size_t		i;
	size_t		length;
	char		*trimmed_str;
	const char	ops[4] = "|&<>";

	if (start >= ft_strlen(str) || end >= ft_strlen(str))
		return (NULL);
	i = 0;
	while (i < 4 && ops[i])
	{
		if (ops[i] == str[end])
			end--;
		i++;
	}
	while (ft_isspace(str[start]) && start <= end)
		start++;
	while (ft_isspace(str[end]) && end >= start)
		end--;
	length = end - start + 1;
	trimmed_str = malloc((length + 1) * sizeof(char));
	if (!trimmed_str)
		error_exit(master, "malloc error in trim_spaces");
	ft_strlcpy(trimmed_str, &str[start], length + 1);
	return (trimmed_str);
}

static int	manage_token(t_master *master, const char *line_read,
	t_token **token_lst)
{
	size_t			i;
	size_t			j;
	char			*data;
	t_token_type	type;

	i = 0;
	type = check_token_type(line_read[i], line_read, &i);
	if (start_operator(type) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (line_read[i])
	{
		j = i;
		type = check_token_type(line_read[i], line_read, &i);
		while (line_read[i] && type == T_BUILTIN)
			type = check_token_type(line_read[++i], line_read, &i);
		data = trim_spaces(master, line_read, j, i - 1);
		create_token_node(master, T_BUILTIN, data, token_lst);
		if (type != T_BUILTIN)
		{
			create_token_node(master, type, NULL, token_lst);
			if (line_read[i])
				i++;
		}
	}
	return (EXIT_SUCCESS);
}

int	launch_lexer(t_master *master, char *line_read, t_token **token_list)
{
	if (is_matched_quotes(line_read) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (manage_token(master, line_read, token_list) == EXIT_FAILURE)
	{
		master->exit_status = 2;
		return (EXIT_FAILURE);
	}
	if ((is_heredoc_pipe(token_list) == EXIT_FAILURE)
		|| (is_clean(token_list) == EXIT_FAILURE))
	{
		master->exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
