/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:28 by chmadran          #+#    #+#             */
/*   Updated: 2023/06/30 18:00:28 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "env.h"
#include "execution.h"
#include "signals.h"
#include "libft.h"
#include "exit.h"

t_master	g_master;

int	main(void)
{
	rl_catch_signals = 0;
	set_sigaction();
	manage_environment(&g_master.env_list);
	while (1)
	{
		g_master.token_list = NULL;
		g_master.line_read = readline("\033[32mminishell:~$ \033[0m");
		if (!g_master.line_read)
			return (handle_eof(), EXIT_SUCCESS);
		if (ft_strlen(g_master.line_read))
			add_history(g_master.line_read);
		if (launch_lexer(g_master.line_read, &g_master.token_list)
			!= EXIT_SUCCESS)
			continue ;
		launch_execution(&g_master);
		free_token_list(g_master.token_list);
		free(g_master.line_read);
	}
}
