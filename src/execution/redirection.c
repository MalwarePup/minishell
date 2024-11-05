/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 11:50:40 by alfloren          #+#    #+#             */
/*   Updated: 2024/11/05 19:04:37 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

void	replace_redir_without_quotes(t_master *master, char **str)
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
	{
		free(*str);
		*str = NULL;
		error_exit(master, "malloc error");
		return ;
	}
	new_str = malloc(sizeof(char) * (ft_strlen(*str) + 1));
	if (!new_str)
	{
		free(test);
		free(*str);
		*str = NULL;
		error_exit(master, "malloc error in lexer_mem.c");
		return ;
	}
	ft_strlcpy(test, *str, ft_strlen(*str) + 1);
	while (test[ij[0]])
	{
		if (to_pass(test, &quote, &ex_quote, &ij[0]))
			continue ;
		new_str[ij[1]++] = test[ij[0]++];
	}
	free(*str);
	*str = new_str;
	new_str[ij[1]] = '\0';
	free(test);
}

static void	redirect(t_master *master, char *file, int flag, int old_fd)
{
	int	new_fd;

	new_fd = open(file, flag, 0644);
	if (new_fd == -1)
		error_exit(master, file);
	if (dup2(new_fd, old_fd) == -1)
		error_exit(master, file);
	close(new_fd);
}

void	launch_redirection(t_master *master, t_token *token)
{
	while (token)
	{
		replace_redir_without_quotes(master, &token->data);
		if (token->type == CMD_RED_IN)
			redirect(master, token->data, O_RDONLY, STDIN_FILENO);
		else if (token->type == CMD_RED_OUT)
			redirect(master, token->data,
				O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
		else if (token->type == CMD_D_RED_OUT)
			redirect(master, token->data,
				O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
		else if (token->type == CMD_D_RED_IN)
			redirect(master, token->data, O_RDONLY, STDIN_FILENO);
		token = token->next;
	}
}
