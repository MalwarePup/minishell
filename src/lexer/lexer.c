/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/05 10:30:06 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

char	*trim_spaces(t_master *master, const char *str, size_t start,
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

int	manage_redirection(const char *line_read, size_t *i, bool redir)
{
	if (is_in_quotes(line_read, i))
		return (EXIT_SUCCESS);
	if (line_read[*i] == '\0')
	{
		if (redir)
			ft_dprintf(STDERR_FILENO, ESTR_OPSTART);
		return (EXIT_FAILURE);
	}
	else if (line_read[*i] == '>' || line_read[*i] == '<')
	{
		if (redir)
			ft_dprintf(STDERR_FILENO, ESTR_UNEXP, line_read[*i]);
		return (EXIT_FAILURE);
	}
	while (!ft_isspace(line_read[*i])
		&& line_read[*i] != '|'
		&& line_read[*i] != '<'
		&& line_read[*i] != '>'
		&& line_read[*i])
		(*i)++;
	return (EXIT_SUCCESS);
}

int	create_node_command(t_master *master, char *line_read,
			t_token **token_list, size_t *j)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*data;
	char	*tmp;

	i = (*j);
	start = 0;
	end = 0;
	data = NULL;
	tmp = NULL;
	while (line_read[i] && line_read[i] != '|')
	{
		next_sign(line_read, &i, &start, &end);
		if (start != end)
			if (create_data_command(line_read, (size_t [2]){start, end},
				&data))
				return (EXIT_FAILURE);
		pass_redirection(line_read, &i);
	}
	if (data && *data)
		create_token_node(master, CMD_OTHERS, data, token_list);
	return (EXIT_SUCCESS);
}

int	create_nodes_redir(t_master *master, char *line_read,
		t_token **token_list, size_t *i)
{
	size_t		k;

	while (line_read[(*i)] && line_read[(*i)] != '|')
	{
		next_sign_redir(line_read, i, &k);
		if (line_read[(*i)] == '|' || line_read[(*i)] == '\0')
			return (finish_line(line_read, i, master, token_list));
		else if (is_in_quotes(line_read, i))
			continue ;
		else
			if (create_redir(master, line_read, token_list,
					(size_t * [2]){i, &k}) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	launch_lexer(t_master *master, char *line_read, t_token **token_list)
{
	size_t	i;

	i = 0;
	if (is_matched_quotes(line_read) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (line_read[i])
	{
		if (create_node_command(master, line_read, token_list, &i)
			!= EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (create_nodes_redir(master, line_read, token_list, &i)
			!= EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (exit_handler(token_list));
}
