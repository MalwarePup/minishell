/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 12:25:37 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 18:39:16 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"

static void	ft_write_stdout(char *str, size_t size, t_master *master)
{
	if (write(STDOUT_FILENO, str, size) == -1)
	{
		cleanup_executable(master);
		error_exit(master, "write (ft_write_stdout)");
	}
}

int	ft_echo(int argc, char **argv, t_master *master)
{
	int		i;
	bool	is_option;

	i = 0;
	is_option = false;
	if (argc > 1 && !ft_strncmp(argv[1], "-n", 2))
	{
		i = 1;
		is_option = true;
	}
	while (++i < argc)
	{
		ft_write_stdout(argv[i], ft_strlen(argv[i]), master);
		if (i < argc - 1 && ft_strlen(argv[i]))
			ft_write_stdout(" ", 1, master);
	}
	if (!is_option)
		ft_write_stdout("\n", 1, master);
	return (EXIT_SUCCESS);
}
