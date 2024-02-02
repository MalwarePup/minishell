/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:51:40 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/02 19:28:04 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	next_sign(char *line_read, size_t *i, size_t *start, size_t *end)
{
	while (ft_isspace(line_read[*i]) && line_read[*i])
			(*i)++;
	*start = *i;
	if (!is_in_quotes(line_read, i))
	{
		while (!ft_isspace(line_read[*i])
			&& line_read[*i] != '|'
			&& line_read[*i] != '<'
			&& line_read[*i] != '>'
			&& line_read[*i])
				(*i)++;
	}
	*end = *i;
}

void	free_exit(char *data, t_master *master)
{
	free(data);
	error_exit(master, "malloc error in create_data_command");
}

// free tmp quand ft_join ne fonctionne pas
int	create_data_command(char *line_read, size_t startend[2],
	t_master *master, char **tmp)
{
	char	*data;

	if (*tmp && **tmp)
	{
		*tmp = ft_strjoin1(*tmp, " ");
		if (!*tmp)
			return (EXIT_FAILURE);
	}
	else
	{
		*tmp = malloc(sizeof(char));
		if (!*tmp)
			return (EXIT_FAILURE);
		*tmp[0] = '\0';
	}
	data = trim_spaces(master, line_read,
			startend[0], startend[1] - 1);
	if (!data)
		return (EXIT_FAILURE);
	*tmp = ft_strjoin2(*tmp, data);
	if (!*tmp)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	pass_redirection(const char *line_read, size_t *i)
{
	if (line_read[*i] == '<' || line_read[*i] == '>')
	{
		if (line_read[*i + 1] == line_read[*i])
			(*i)++;
		(*i)++;
		manage_redirection(line_read, i, false);
	}
}

// void	if_redir_command(char *line_read, size_t *i)
// {
// 	if (line_read[*i] == '<' || line_read[*i] == '>')
// 	{
// 		if (line_read[*i + 1] == line_read[*i])
// 			(*i)++;
// 		(*i)++;
// 		manage_redirection(line_read, i, false);
// 	}
// }
