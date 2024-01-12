/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:06:28 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 19:25:29 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"

static bool	is_numeric_argument(const char *arg)
{
	if (arg[0] == '-' || arg[0] == '+')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (false);
		arg++;
	}
	return (true);
}

static int	check_arguments(int argc, char **argv)
{
	if (argc > 1 && !is_numeric_argument(argv[1]))
	{
		ft_dprintf(STDERR_FILENO, NUM_ARG_ERR, argv[1]);
		return (2);
	}
	else if (argc > 2)
	{
		ft_putstr_fd(TOO_MANY_ARGS_ERR, STDERR_FILENO);
		g_exit_status = 1;
		return (257);
	}
	else if (argc > 1)
		return (ft_latoi(argv[1]) % 256);
	return (258);
}

void	ft_exit(t_master *master, int argc, char **argv)
{
	int	g_exit_status;

	g_exit_status = 0;
	if (argc > 1)
		g_exit_status = check_arguments(argc, argv);
	if (write(STDOUT_FILENO, "exit\n", 5) == -1)
	{
		cleanup_before_exit(master);
		cleanup_executable(master);
		perror("write (handle_eof)");
		exit(EXIT_FAILURE);
	}
	if (g_exit_status != 257)
	{
		cleanup_before_exit(master);
		cleanup_executable(master);
		exit(g_exit_status);
	}
}
