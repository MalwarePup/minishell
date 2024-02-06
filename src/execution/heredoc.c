/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:00:55 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/06 12:40:35 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

t_token	*find_heredoc(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		while (current->redir)
		{
			if (current->redir->type == CMD_D_RED_IN)
				return (current);
			current->redir = current->redir->next;
		}
		current = current->next;
	}
	return (NULL);
}

void	read_heredoc_into_file(t_master *master, int fd, const char *delimiter)
{
	char	*line_read;

	while (1)
	{
		line_read = readline("> ");
		if (!line_read)
		{
			write(STDOUT_FILENO, "warning\n", 8);
			free(line_read);
			break ;
		}
		else if (!ft_strncmp(line_read, delimiter, ft_strlen(delimiter)))
		{
			free(line_read);
			break ;
		}
		if (write(fd, line_read, ft_strlen(line_read)) == -1)
			error_exit(master, "write (read_heredoc_into_file)");
		if (write(fd, "\n", 1) == -1)
			error_exit(master, "write (read_heredoc_into_file)");
		free(line_read);
	}
}

void	launch_heredoc(t_master *master)
{
	t_token	*heredoc;
	char	*filename;
	char	*line;
	int		fd;


	filename = "/tmp/heredoc_tmp";
	heredoc = find_heredoc(master->token_list);
	if (heredoc)
	{

		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			error_exit(master, "open (read_heredoc_into_file)");
		line = NULL;
		read_heredoc_into_file(master, fd, heredoc->redir->data);
		close(fd);
		heredoc->redir->data = filename;
	}
}
