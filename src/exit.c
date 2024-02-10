/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 12:43:03 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/10 13:14:27 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"
#include "ft_dprintf.h"

void	error_exit(t_master *master, char *error_str, bool free_all_exec)
{
	perror(error_str);
	if (free_all_exec)
		cleanup_executable(master);
	cleanup_before_exit(master);
	exit(EXIT_FAILURE);
}

void	ft_error_exit(t_master *master, char *error_str, int errnum,
	bool free_all_exec)
{
	ft_dprintf(STDERR_FILENO, "%s: %s\n", error_str, strerror(errnum));
	if (free_all_exec)
		cleanup_executable(master);
	cleanup_before_exit(master);
	exit(EXIT_FAILURE);
}

void	handle_eof(t_master *master)
{
	rl_clear_history();
	free_environment_list(master->env_list);
	free(master->line_read);
	if (write(STDOUT_FILENO, "\nexit\n", 6) == -1)
	{
		perror("write (handle_eof)");
		exit(EXIT_FAILURE);
	}
}
