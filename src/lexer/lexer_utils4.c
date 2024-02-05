/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:34:53 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/05 10:41:45 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	next_sign_redir(char *line_read, size_t *i, size_t	*k)
{
	while (ft_isspace(line_read[(*i)])
		|| (line_read[(*i)] != '|'
			&& line_read[(*i)] != '<'
			&& line_read[(*i)] != '>'
			&& line_read[(*i)] != '\''
			&& line_read[(*i)] != '\"'
			&& line_read[(*i)]))
		(*i)++;
	(*k) = (*i);
}

t_cmd_type	if_redir(char *line_read, size_t *i, t_cmd_type type)
{
	if (line_read[(*i)] == '<')
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
	return (type);
}

int	finish_line(char *line_read, size_t *i, t_master *master,
	t_token **token_list)
{
	t_cmd_type	type;

	type = CMD_OTHERS;
	if (line_read[(*i)] == '|')
	{
		create_token_node(master, CMD_PIPE, NULL, token_list);
		(*i)++;
		(*token_list)->last->redir = NULL;
	}
	return (EXIT_SUCCESS);
}

char	*trim_spaces1(const char *str, size_t startend[2])
{
	size_t		i;
	size_t		length;
	char		*trimmed_str;
	const char	ops[4] = "|&<>";

	if (startend[0] >= ft_strlen(str) || startend[1] >= ft_strlen(str))
		return (NULL);
	i = 0;
	while (i < 4 && ops[i])
	{
		if (ops[i] == str[startend[1]])
			startend[1]--;
		i++;
	}
	while (ft_isspace(str[startend[0]]) && startend[0] <= startend[1])
		startend[0]++;
	while (ft_isspace(str[startend[1]]) && startend[1] >= startend[0])
		startend[1]--;
	length = startend[1] - startend[0] + 1;
	trimmed_str = malloc((length + 1) * sizeof(char));
	if (!trimmed_str)
		return (NULL);
	ft_strlcpy(trimmed_str, &str[startend[0]], length + 1);
	return (trimmed_str);
}

int	create_redir(t_master *master, char *line_read, t_token **token_list,
	size_t *ik[2])
{
	char		*data;
	t_cmd_type	type;

	type = CMD_OTHERS;
	if (line_read[(*ik[0])] == '>' || line_read[(*ik[0])] == '<')
			type = if_redir(line_read, ik[0], type);
	if (ik[0] != ik[1])
	{
		*ik[1] = *ik[0];
		if (manage_redirection(line_read, ik[0], true) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		data = trim_spaces(master, line_read, *ik[1], *ik[0] - 1);
		if (!data)
			return (EXIT_FAILURE);
		if (!(*token_list))
			create_token_node(master, type, NULL, token_list);
		create_token_node(master, type, data, &(*token_list)->last->redir);
	}
	return (EXIT_SUCCESS);
}
