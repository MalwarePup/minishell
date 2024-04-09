/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:32:49 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/09 10:43:04 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int	count_pipe(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->type == CMD_PIPE)
			i++;
		token = token->next;
	}
	return (i + 1);
}

int is_different(int fd) {
    int copy = fcntl(fd, F_DUPFD, 0);
    if (copy == -1) {
		// error_exit(master, "fcntl (is_different)");
		return 0;
    }
    int is_different = (copy != STDOUT_FILENO);
    close(copy);
    return is_different;
}

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

static void	replace_redir(t_master *master, char **str)
{
	char	*new_str;
	char	*test;
	char	quote;
	char	ex_quote;
	size_t	ij[2];

	quote = 0;
	ex_quote = 0;
	ij[0] = 0;
	ij[1] = 0;
	test = malloc(sizeof(char) * (ft_strlen(*str) + 1));
	if (!test)
		return (free(*str), *str = NULL, error_exit(master, "malloc error"));
	new_str = malloc(sizeof(char) * (ft_strlen(*str) + 1));
	if (!new_str)
		return (free(test), free(*str), *str = NULL,
			error_exit(master, "malloc error in lexer_mem.c"));
	ft_strlcpy(test, *str, ft_strlen(*str) + 1);
	while (test[ij[0]])
	{
		if (to_pass(test, &quote, &ex_quote, &ij[0]))
			continue ;
		new_str[ij[1]++] = test[ij[0]++];
	}
	return (free(*str), *str = new_str, new_str[ij[1]] = '\0', free(test));
}

static bool	redirect(t_master *master, char *file, int flag, int fd)
{
	int	new_fd;

	new_fd = open(file, flag, 0644);
	if (new_fd == -1)
	{
		perror(file);
		master->exit_status = 1;
		return false;
	}
	if (dup2(new_fd, fd) == -1)
		error_exit(master, file);
	close(new_fd);
	return true;
}

int	launch_nocommand(t_master *master, t_token *tmp)
{
	t_token	*token;
	bool is_input;
	bool is_output;
	int stdin;
	int stdout;

	token = tmp;
	is_input = false;
	is_output = false;
	stdin = dup(STDIN_FILENO);
	stdout = dup(STDOUT_FILENO);
	while (token)
	{
		replace_redir(master, &token->data);
		if (token->type == CMD_RED_IN)
		{
			if (redirect(master, token->data, O_RDONLY, STDIN_FILENO) == true)
				is_input = true;
			else
				return (EXIT_FAILURE);
		}
		else if (token->type == CMD_RED_OUT)
		{
			if (redirect(master, token->data,
				O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO) == true)
				is_output = true;
			else
				return (EXIT_FAILURE);
		}
		else if (token->type == CMD_D_RED_OUT)
		{
			if (redirect(master, token->data,
				O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO) == true)
				is_output = true;
			else
				return (EXIT_FAILURE);
		}
		else if (token->type == CMD_D_RED_IN)
		{
			if (redirect(master, token->data, O_RDONLY, STDIN_FILENO) == true)
				is_input = true;
			else
				return (EXIT_FAILURE);
		}
		token = token->next;
	}
	if (is_input == true)
		dup2(stdin, STDIN_FILENO);
	if (is_output == true)
		dup2(stdout, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int	no_command(t_master *master, t_token **token)
{
	if ((*token)->type == CMD_NOCMD)
	{
		if (launch_nocommand(master, (*token)->redir) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		*token = (*token)->next;
		if (*token && (*token)->type == CMD_PIPE)
			*token = (*token)->next;
		return (EXIT_MISUSE);
	}
	return (EXIT_SUCCESS);
}