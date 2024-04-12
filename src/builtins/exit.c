/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:06:28 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/12 15:46:12 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_dprintf.h"
#include "libft.h"
#include "minishell.h"

static bool	is_numeric_argument(const char *arg)
{
	bool		is_negative;
	long long	res;

	res = 0;
	is_negative = (arg[0] == '-');
	if (is_negative || arg[0] == '+')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (false);
		if (is_negative && res < (LLONG_MIN + (*arg - '0')) / 10)
			return (false);
		if (!is_negative && res > (LLONG_MAX - (*arg - '0')) / 10)
			return (false);
		if (is_negative)
			res = res * 10 - (*arg - '0');
		else
			res = res * 10 + (*arg - '0');
		arg++;
	}
	return (true);
}

int	builtin_exit(t_master *master, int argc, char **argv)
{
	if (argc > 1 && (!argv[1][0] || !is_numeric_argument(argv[1])))
	{
		ft_dprintf(STDERR_FILENO, ESTR_NUM_ARG, argv[1]);
		master->last_command_exit_value = MISUSE;
	}
	else if (argc > 2)
	{
		ft_putstr_fd(ESTR_EXIT_TOO_MANY_ARGS, STDERR_FILENO);
		return (1);
	}
	else if (argc > 1)
		master->last_command_exit_value = ft_atoi(argv[1]) % 256;
	if (isatty(STDIN_FILENO) && master->exec->pipefd[0] == -1
		&& write(STDERR_FILENO, "exit\n", (size_t)5) == -1)
		error_exit(master, "write (builtin_exit)");
	cleanup_before_exit(master);
	exit(master->last_command_exit_value);
}
