/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 12:25:37 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 21:14:59 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

static void	ft_write_stdout(char *str, size_t size, t_master *master)
{
	if (write(STDOUT_FILENO, str, size) == -1)
		error_exit(master, "write (ft_write_stdout)");
}

bool	check_option(int argc, char **argv, int *i)
{
	int		j;
	bool	is_option;

	is_option = false;
	if (argc > 1)
	{
		*i = 1;
		while (*i < argc && argv[*i][0] == '-')
		{
			j = 1;
			while (argv[*i][j] == 'n')
				j++;
			if (argv[*i][j] == '\0')
			{
				if (j != 1)
					is_option = true;
				(*i)++;
			}
			else
				break ;
		}
	}
	return (is_option);
}

int	ft_echo(int argc, char **argv, t_master *master)
{
	int		i;
	bool	is_option;

	i = 1;
	is_option = check_option(argc, argv, &i);
	while (i < argc)
	{
		ft_write_stdout(argv[i], ft_strlen(argv[i]), master);
		if (i + 1 < argc)
			ft_write_stdout(" ", 1, master);
		i++;
	}
	if (!is_option)
		ft_write_stdout("\n", 1, master);
	return (EXIT_SUCCESS);
}
