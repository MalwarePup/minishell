/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/31 12:20:17 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

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

// static int	manage_token(t_master *master, const char *line_read,
// 	t_token **token_lst)
// {
// 	size_t			i;
// 	size_t			j;
// 	char			*data;
// 	t_token_type	type;

// 	i = 0;
// 	type = check_token_type(line_read[i], line_read, &i);
// 	if (start_operator(type) == EXIT_FAILURE)
// 		return (EXIT_FAILURE);
// 	while (line_read[i])
// 	{
// 		j = i;
// 		type = check_token_type(line_read[i], line_read, &i);
// 		while (line_read[i] && type == T_BUILTIN)
// 			type = check_token_type(line_read[++i], line_read, &i);
// 		data = trim_spaces(master, line_read, j, i - 1);
// 		create_token_node(master, T_BUILTIN, data, token_lst);
// 		if (type != T_BUILTIN)
// 		{
// 			create_token_node(master, type, NULL, token_lst);
// 			if (line_read[i] == ' ')
// 				i++;
// 		}
// 	}
// 	return (EXIT_SUCCESS);
// }

// int	launch_lexer(t_master *master, char *line_read, t_token **token_list)
// {
// 	if (is_matched_quotes(line_read) == EXIT_FAILURE)
// 		return (EXIT_FAILURE);
// 	if (manage_token(master, line_read, token_list) == EXIT_FAILURE)
// 	{
// 		g_exit_status = 2;
// 		return (EXIT_FAILURE);
// 	}
// 	if ((is_heredoc_pipe(token_list) == EXIT_FAILURE)
// 		|| (is_clean(token_list) == EXIT_FAILURE))
// 	{
// 		g_exit_status = 2;
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

t_cmd_type	check_type(char c, const char *line_read, size_t *i)
{
	t_cmd_type		token_type;

	token_type = isnot_builtins(c, line_read, i);
	if (token_type != CMD_OTHERS)
		return (token_type);
	return (is_builtin(line_read, i));
}

static int	manage_redirection(const char *line_read, size_t *i)
{
	if (is_in_quotes(line_read, i))
		return (CMD_OTHERS);
	if (line_read[*i] == '\0')
	{
		printf(ESTR_OPSTART);
		return (EXIT_FAILURE);
	}
	while (line_read[*i] == ' ' && line_read[*i])
		(*i)++;
	while (line_read[*i] != ' '
		&& line_read[*i] != '|'
		&& line_read[*i] != '<'
		&& line_read[*i] != '>'
		&& line_read[*i])
		(*i)++;
	return (CMD_OTHERS);
}

int	launch_lexer(t_master *master, char *line_read, t_token **token_lst)
{
	size_t			i;
	size_t			j;
	char			*data;
	t_cmd_type	type;

	i = 0;
	type = CMD_OTHERS;
	if (is_matched_quotes(line_read) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (line_read[i])
	{
		while (line_read[i] == ' ' && line_read[i])
			i++;
		j = i;
		type = check_type(line_read[i], line_read, &i);
		if (type != CMD_OTHERS && type != CMD_PIPE)
		{
			j = i;
			if ((manage_redirection(line_read, &i) == EXIT_FAILURE))
				return (EXIT_FAILURE);
		}
		data = trim_spaces(master, line_read, j, i - 1);
		create_token_node(master, type, data, token_lst);
	}
	return (exit_handler(token_lst));
}
