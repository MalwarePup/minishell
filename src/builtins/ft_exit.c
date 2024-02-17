/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:06:28 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/17 10:35:23 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"
#include "ft_dprintf.h"

static bool	is_numeric_argument(const char *arg)
{
	bool	is_negative;

	is_negative = (arg[0] == '-');
	if (is_negative || arg[0] == '+')
		arg++;
	if (!is_negative)
	{
		if (ft_strcmp(arg, STR_BEYOND_LLONG_MAX) == 0)
			return (false);
	}
	else
	{
		if (ft_strcmp(arg, STR_BEYOND_LLONG_MIN) == 0)
			return (false);
	}
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (false);
		arg++;
	}
	return (true);
}

static void	check_arguments(t_master *master, int argc, char **argv)
{
	if (argc > 1 && !is_numeric_argument(argv[1]))
	{
		ft_dprintf(STDERR_FILENO, ESTR_NUM_ARG, argv[1]);
		master->exit_status = 2;
	}
	else if (argc > 2)
	{
		ft_putstr_fd(ESTR_EXIT_TOO_MANY_ARGS, STDERR_FILENO);
		master->exit_status = 1;
	}
	else if (argc > 1)
		master->exit_status = (ft_latoi(argv[1]) % 256);
}

void	ft_exit(t_master *master, int argc, char **argv)
{
	master->exit_status = 0;
	if (argc > 1)
		check_arguments(master, argc, argv);
	if (write(STDOUT_FILENO, "exit\n", (size_t)5) == -1)
		error_exit(master, "write (ft_exit)");
	cleanup_before_exit(master);
	exit(master->exit_status);
}
