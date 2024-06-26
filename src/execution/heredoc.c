/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:00:55 by alfloren          #+#    #+#             */
/*   Updated: 2024/04/13 15:53:32 by ladloff          ###   ########.fr       */
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

static bool	is_expandable(char *str)
{
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			return (false);
		str++;
	}
	return (true);
}

static void	read_heredoc_into_file(t_master *master, int fd,
	const char *delimiter, bool expand)
{
	char	*line_read;

	while (1)
	{
		line_read = readline("> ");
		if (!line_read)
		{
			if (master->last_command_exit_value != 256)
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
		if (expand)
			launch_expansion(master, &line_read);
		if (write(fd, line_read, ft_strlen(line_read)) == -1
			|| write(fd, "\n", 1) == -1)
			error_exit(master, "write (read_heredoc_into_file)");
		free(line_read);
	}
}

static int	create_file(t_master *master, t_token **token, int i)
{
	int		fd;
	char	*filename;
	char	*itoa;
	bool	expand;

	itoa = ft_itoa(i);
	if (!itoa)
		error_exit(master, "ft_itoa (create_file)");
	filename = ft_strjoin3("/tmp/heredoc_", itoa);
	if (!filename)
	{
		free(itoa);
		error_exit(master, "ft_strjoin3 (create_file)");
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		error_exit(master, "open (read_heredoc_into_file)");
	}
	expand = is_expandable((*token)->data);
	replace_redir_without_quotes(master, &((*token)->data));
	read_heredoc_into_file(master, fd, (*token)->data, expand);
	close(fd);
	return (free((*token)->data), (*token)->data = filename, 0);
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
