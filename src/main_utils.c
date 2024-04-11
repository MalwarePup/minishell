/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 12:43:03 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/11 14:51:43 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"
#include "ft_dprintf.h"

void	error_exit(t_master *master, char *error_str)
{
	perror(error_str);
	cleanup_before_exit(master);
	exit(EXIT_FAILURE);
}

void	handle_eof(t_master *master)
{
	rl_clear_history();
	free(master->line_read);
	free_environment_list(&master->env);
	if (isatty(STDIN_FILENO) && write(STDERR_FILENO, "\nexit\n", 6) == -1)
	{
		perror("write (handle_eof)");
		exit(EXIT_FAILURE);
	}
}
