/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:00:55 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/15 12:12:54 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"
#include "ft_dprintf.h"

void	read_heredoc_into_file(t_master *master, int fd, const char *delimiter)
{
	char	*line_read;

	while (1)
	{
		line_read = readline("> ");
		if (!line_read || master->exit_status == 131)
		{
			if (master->prev_exit_status != 131)
				ft_dprintf(STDERR_FILENO, ESTR_HEREDOC_P1 ESTR_HEREDOC_P2,
					master->line_count, delimiter);
			free(line_read);
			break ;
		}
		else if (!ft_strncmp(line_read, delimiter, ft_strlen(delimiter) + 1))
		{
			free(line_read);
			break ;
		}
		if (write(fd, line_read, ft_strlen(line_read)) == -1)
			error_exit(master, "write (read_heredoc_into_file)", false);
		if (write(fd, "\n", 1) == -1)
			error_exit(master, "write (read_heredoc_into_file)", false);
		free(line_read);
	}
}

int	create_file(t_master *master, t_token **token, int i)
{
	char	*filename;
	int		fd;

	filename = create_filename(i);
	if (!filename)
		ft_error_exit(master, "create_filename (create_file)", ENOMEM, false);
	fd = create_or_open_file(&filename, &i);
	if (fd == -1)
	{
		free(filename);
		error_exit(master, "create_or_open_file (create_file)", false);
	}
	read_heredoc_into_file(master, fd, (*token)->data);
	free((*token)->data);
	(*token)->data = filename;
	return (fd);
}

void	launch_heredoc(t_master *master)
{
	int		i;
	int		original_stdin;
	t_token	*redir;
	t_token	*current;

	i = 0;
	current = master->token;
	master->exec->heredoc = true;
	set_sigaction_heredoc(master);
	original_stdin = dup(STDIN_FILENO);
	while (current)
	{
		redir = current->redir;
		while (redir)
		{
			if (redir->type == CMD_D_RED_IN)
				create_file(master, &redir, i++);
			redir = redir->next;
		}
		current = current->next;
	}
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	restore_sigaction(master);
}
