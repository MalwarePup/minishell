/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:28 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/18 22:43:30 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "libft.h"

int	*g_exit_status = NULL;

static int	shell_loop(t_master *master)
{
	while (1)
	{
		if (master->last_command_exit_value == EXIT_INTERRUPTED_HEREDOC
			|| master->last_command_exit_value == EXIT_INTERRUPTED_TEMP)
			master->exit_status = EXIT_INTERRUPTED;
		master->exec = NULL;
		master->token = NULL;
		master->pid_list = NULL;
		master->last_command_exit_value = master->exit_status;
		master->exit_status = 0;
		restore_sigaction(master);
		master->line_read = readline("\033[32mminishell:~$ \033[0m");
		if (!master->line_read)
			return (handle_eof(master), EXIT_SUCCESS);
		master->line_count++;
		if (*master->line_read)
			add_history(master->line_read);
		launch_expansion(master, &master->line_read);
		if (is_matched_quotes(master, master->line_read)
			&& !launch_lexer(master, master->line_read, &master->token))
			launch_execution(master);
		free(master->line_read);
		free_token(&master->token);
		free(master->pid_list);
		free(master->exec);
	}
}

int	main(void)
{
	t_master	master;

	rl_catch_signals = 0;
	master.env_list = NULL;
	master.pid_list = NULL;
	master.exit_status = 0;
	master.last_command_exit_value = 0;
	master.line_count = 0;
	g_exit_status = &master.last_command_exit_value;
	set_sigaction(&master);
	manage_environment(&master, &master.env_list);
	return (shell_loop(&master));
}
