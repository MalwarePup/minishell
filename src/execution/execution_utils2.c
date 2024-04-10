/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:32:49 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/10 08:50:46 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

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

void	replace_redir(t_master *master, char **str)
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
		return (free(test), free(*str), *str = NULL, error_exit(master,
				"malloc error in lexer_mem.c"));
	ft_strlcpy(test, *str, ft_strlen(*str) + 1);
	while (test[ij[0]])
	{
		if (to_pass(test, &quote, &ex_quote, &ij[0]))
			continue ;
		new_str[ij[1]++] = test[ij[0]++];
	}
	return (free(*str), *str = new_str, new_str[ij[1]] = '\0', free(test));
}

bool	redirect_cmd(t_master *master, char *file, int flag, int fd)
{
	int	new_fd;

	new_fd = open(file, flag, 0644);
	if (new_fd == -1)
	{
		perror(file);
		master->exit_status = 1;
		return (false);
	}
	if (dup2(new_fd, fd) == -1)
		error_exit(master, file);
	close(new_fd);
	return (true);
}

int	launch_nocommand(t_master *master, t_token *tmp)
{
	t_token	*token;
	bool	is_input;
	bool	is_output;
	int		stdin;
	int		stdout;

	token = tmp;
	is_input = false;
	is_output = false;
	stdin = dup(STDIN_FILENO);
	stdout = dup(STDOUT_FILENO);
	while (token)
	{
		if (handle_redir(master, token, &is_input, &is_output))
			return (1);
		token = token->next;
	}
	if (is_input == true)
		if (dup2(stdin, STDIN_FILENO) == -1)
			error_exit(master, "dup2: stdin");
	if (is_output == true)
		if (dup2(stdout, STDOUT_FILENO) == -1)
			error_exit(master, "dup2: stdout");
	return (0);
}

int	no_command(t_master *master, t_token **token)
{
	if ((*token)->type == CMD_NOCMD)
	{
		if (launch_nocommand(master, (*token)->redir))
			return (1);
		*token = (*token)->next;
		if (*token && (*token)->type == CMD_PIPE)
			*token = (*token)->next;
		return (MISUSE);
	}
	return (0);
}
