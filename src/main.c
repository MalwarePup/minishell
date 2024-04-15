/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:28 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/15 10:27:24 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "get_next_line.h"
#include "minishell.h"
#include "libft.h"

int	*g_exit_status = NULL;

static void	init_master(t_master *master)
{
	if (master->last_command_exit_value == 130
		|| master->last_command_exit_value == 256)
		master->exit_status = 130;
	else if (master->last_command_exit_value == 131)
		master->exit_status = 131;
	master->exec = NULL;
	master->token = NULL;
	master->pid_list = NULL;
	if (master->reset_exit_status)
		master->last_command_exit_value = master->exit_status;
	master->exit_status = 0;
}

static void	free_master(t_master *master)
{
	free(master->line_read);
	free_token(&master->token);
	free(master->pid_list);
	free(master->exec);
}

static int	shell_loop(t_master *master)
{
	while (1)
	{
		init_master(master);
		restore_sigaction(master);
		if (isatty(STDIN_FILENO))
			master->line_read = readline("\033[32mminishell:~$ \033[0m");
		else
			master->line_read = get_next_line(STDIN_FILENO);
		if (!master->line_read)
			return (handle_eof(master), master->last_command_exit_value);
		master->line_count++;
		if (*master->line_read)
		{
			add_history(master->line_read);
			launch_expansion(master, &master->line_read);
			if (is_matched_quotes(master, master->line_read)
				&& !launch_lexer(master))
				launch_execution(master);
			master->reset_exit_status = true;
		}
		else
			master->reset_exit_status = false;
		free_master(master);
	}
}

int	main(int argc, char *argv[])
{
	t_master	master;
	extern char	**environ;

	(void)argc;
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	ft_memset(&master, 0, sizeof(t_master));
	master.reset_exit_status = true;
	master.program_name = argv[0];
	g_exit_status = &master.last_command_exit_value;
	if (!*environ)
		master.env_empty = true;
	if (set_sigaction(&master))
		return (EXIT_FAILURE);
	if (manage_environment(&master.env))
	{
		free_environment_list(&master.env);
		return (EXIT_FAILURE);
	}
	return (shell_loop(&master));
}
