/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:06:28 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/05 21:18:18 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "exit.h"

void	ft_exit(void)
{
	cleanup_before_exit();
	cleanup_executable();
	if (write(STDOUT_FILENO, "exit\n", 5) == -1)
	{
		perror("write (handle_eof)");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
