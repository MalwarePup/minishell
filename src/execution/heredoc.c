/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:00:55 by alfloren          #+#    #+#             */
/*   Updated: 2024/02/10 13:19:18 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

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
			error_exit(master, "write (read_heredoc_into_file)", false);
		if (write(fd, "\n", 1) == -1)
			error_exit(master, "write (read_heredoc_into_file)", false);
		free(line_read);
	}
}

void	create_file(t_master *master, t_token **token, int i)
{
	int		fd;
	char	*filename;
	char	*itoa;

	itoa = ft_itoa(i);
	if (!itoa)
		ft_error_exit(master, "ft_itoa (create_file)", ENOMEM, false);
	filename = ft_strjoin3("/tmp/heredoc_", itoa);
	if (!filename)
	{
		free(itoa);
		ft_error_exit(master, "ft_strjoin3 (create_file)", ENOMEM, false);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		error_exit(master, "open (read_heredoc_into_file)", false);
	}
	read_heredoc_into_file(master, fd, (*token)->data);
	close(fd);
	free((*token)->data);
	(*token)->data = filename;
	create_token_node(master, CMD_RED_IN, ft_strdup(filename),
		&(master->exec->heredoc_list));
}

void	launch_heredoc(t_master *master)
{
	t_token	*current;
	t_token	*redir;
	int		i;

	current = master->token_list;
	i = 0;
	while (current)
	{
		redir = current->redir;
		while (redir)
		{
			if (redir->type == CMD_D_RED_IN)
			{
				create_file(master, &redir, i);
				i++;
			}
			redir = redir->next;
		}
		current = current->next;
	}
}
