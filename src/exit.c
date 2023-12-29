/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 12:43:03 by chmadran          #+#    #+#             */
/*   Updated: 2023/07/05 21:37:37 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "exit.h"
#include "env.h"
#include "ft_dprintf.h"

void	error_exit(char *error_str)
{
	perror(error_str);
	cleanup_before_exit();
	exit(EXIT_FAILURE);
}

void	ft_error_exit(char *error_str, int errnum)
{
	ft_dprintf(STDERR_FILENO, "%s: %s\n", error_str, strerror(errnum));
	cleanup_before_exit();
	exit(EXIT_FAILURE);
}

void	handle_eof(void)
{
	rl_clear_history();
	free_environment_list(g_master.env_list);
	free(g_master.line_read);
	if (write(STDOUT_FILENO, "\nexit\n", 6) == -1)
	{
		perror("write (handle_eof)");
		exit(EXIT_FAILURE);
	}
}
