/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:41:22 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/02 16:09:31 by alfloren         ###   ########.fr       */
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
void	create_node_command(t_master *master, char *line_read,
			t_token **token_list, size_t *j)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*data;

	i = (*j);
	start = 0;
	end = 0;
	data = NULL;
	while (line_read[i] && line_read[i] != '|')
	{
		while (ft_isspace(line_read[i]) && line_read[i])
			i++;
		start = i;
		if (!is_in_quotes(line_read, &i))
		{
			while (!ft_isspace(line_read[i])
				&& line_read[i] != '|'
				&& line_read[i] != '<'
				&& line_read[i] != '>'
				&& line_read[i])
				i++;
		}
		end = i;
		if (start != end)
		{
			if (data && *data)
				data = ft_strjoin1(data, " ");
			else
			{
				data = malloc(sizeof(char));
				data[0] = '\0';
			}
			data = ft_strjoin2(data,
					trim_spaces(master, line_read, start, end - 1));
		}
		if (line_read[i] == '<' || line_read[i] == '>')
		{
			if (line_read[i + 1] == line_read[i])
				i++;
			i++;
			manage_redirection(line_read, &i, false);
		}
	}
	if (data && *data)
		create_token_node(master, CMD_OTHERS, data, token_list);
}

int	create_nodes_redir(t_master *master, char *line_read, t_token **token_list, size_t *i)
{
	size_t		j;
	size_t		k;
	char		*data;
	t_cmd_type	type;

	while (line_read[(*i)] && line_read[(*i)] != '|')
	{
		while (ft_isspace(line_read[(*i)])
			|| (line_read[(*i)] != '|'
				&& line_read[(*i)] != '<'
				&& line_read[(*i)] != '>'
				&& line_read[(*i)] != '\''
				&& line_read[(*i)] != '\"'
				&& line_read[(*i)]))
			(*i)++;
		k = (*i);
		if (line_read[(*i)] == '\0')
			return (EXIT_SUCCESS);
		else if (line_read[(*i)] == '|')
		{
			create_token_node(master, CMD_PIPE, NULL, token_list);
			(*i)++;
			(*token_list)->last->redir = NULL;
			return (EXIT_SUCCESS);
		}
		else if (is_in_quotes(line_read, i))
			continue ;
		else if (line_read[(*i)] == '<')
		{
			if (line_read[(*i) + 1] != '<')
				type = CMD_RED_IN;
			else
			{
				(*i)++;
				type = CMD_D_RED_IN;
			}
			(*i)++;
		}
		else if (line_read[(*i)] == '>')
		{
			if (line_read[(*i) + 1] != '>')
				type = CMD_RED_OUT;
			else
			{
				(*i)++;
				type = CMD_D_RED_OUT;
			}
			(*i)++;
		}
		if (k != (*i))
		{
			j = (*i);
			if (manage_redirection(line_read, i, true) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
			data = trim_spaces(master, line_read, j, (*i) - 1);
			if (!(*token_list))
				create_token_node(master, type, NULL, token_list);
			create_token_node(master, type, data, &(*token_list)->last->redir);
		}
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
		create_node_command(master, line_read, token_list, &i);
		if (create_nodes_redir(master, line_read, token_list, &i)
			!= EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (exit_handler(token_list));
}

// int	launch_lexer(t_master *master, char *line_read, t_token **token_lst)
// {
// 	size_t			i;
// 	size_t			j;
// 	char			*data;
// 	t_cmd_type		type;

// 	i = 0;
// 	type = CMD_OTHERS;
// 	if (is_matched_quotes(line_read) == EXIT_FAILURE)
// 		return (EXIT_FAILURE);
// 	while (line_read[i])
// 	{
// 		printf("line_read[%zu] = %c\n", i, line_read[i]);
// 		while ((line_read[i] == ' ' || line_read[i] == '\t') && line_read[i])
// 			i++;
// 		j = i;
// 		type = check_type(line_read[i], line_read, &i);
// 		if (type != CMD_OTHERS && type != CMD_PIPE)
// 		{
// 			j = i;
// 			if ((manage_redirection(line_read, &i) == EXIT_FAILURE))
// 				return (EXIT_FAILURE);
// 		}
// 		data = trim_spaces(master, line_read, j, i - 1);
// 		create_token_node(master, type, data, token_lst);
// 	}
// 	return (exit_handler(token_lst));
// }

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

// t_cmd_type	check_type(char c, const char *line_read, size_t *i)
// {
// 	t_cmd_type		token_type;

// 	token_type = isnot_builtins(c, line_read, i);
// 	if (token_type != CMD_OTHERS)
// 		return (token_type);
// 	return (is_builtin(line_read, i));
// }
