/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:28 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/06 17:22:19 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "libft.h"

int	g_exit_status = 0;

int	main(void)
{
	t_master	master;

	rl_catch_signals = 0;
	master.env_list = NULL;
	set_sigaction();
	manage_environment(&master, &master.env_list);
	while (1)
	{
		master.token_list = NULL;
		master.line_read = readline("\033[32mminishell:~$ \033[0m");
		if (!master.line_read)
			return (handle_eof(&master), EXIT_SUCCESS);
		if (*master.line_read)
			add_history(master.line_read);
		if (launch_lexer(&master, master.line_read, &master.token_list)
			== EXIT_SUCCESS)
			launch_execution(&master);
		free(master.exec);
		free_token_list(master.token_list);
		free(master.line_read);
	}
}
