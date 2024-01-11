/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:28 by ladloff          #+#    #+#             */
/*   Updated: 2023/06/30 18:00:28 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "libft.h"

int	main(void)
{
	t_master	master;

	master.env_list = NULL;
	rl_catch_signals = 0;
	set_sigaction();
	manage_environment(&master, &master.env_list);
	while (1)
	{
		master.token_list = NULL;
		master.line_read = readline("\033[32mminishell:~$ \033[0m");
		if (!master.line_read)
			return (handle_eof(&master), EXIT_SUCCESS);
		if (ft_strlen(master.line_read))
			add_history(master.line_read);
		if (launch_lexer(&master, master.line_read, &master.token_list)
			!= EXIT_SUCCESS)
			continue ;
		launch_execution(&master);
		free_token_list(master.token_list);
		free(master.line_read);
	}
}
