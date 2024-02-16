/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:28 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/16 12:18:55 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "libft.h"
#include <errno.h>

int	*g_exit_status = NULL;

static void	launch_expansion_main(t_master *master)
{
	master->exec = ft_calloc(1, sizeof(t_exec));
	if (!master->exec)
		ft_error_exit(master, "ft_calloc (ft_calloc)", ENOMEM, false);
	master->exec->argv = malloc(sizeof(char *) * 2);
	if (!master->exec->argv)
		return (free(master->exec),
			ft_error_exit(master, "malloc (master->exec->argv)",
				ENOMEM, false));
	master->exec->argv[0] = master->line_read;
	master->exec->argv[1] = NULL;
	master->exec->argc = 1;
	launch_expansion(master);
	master->line_read = ft_strdup(master->exec->argv[0]);
	free_string_array(master->exec->argv);
	free(master->exec);
	master->exec = NULL;
	if (!master->line_read)
	{
		free(master->line_read);
		master->line_read = NULL;
		return ;
	}
}

static int	shell_loop(t_master *master)
{
	while (1)
	{
		if (master->prev_exit_status == 131 || master->prev_exit_status == 132)
			master->exit_status = 130;
		master->exec = NULL;
		master->token = NULL;
		master->prev_exit_status = master->exit_status;
		master->exit_status = 0;
		restore_sigaction(master);
		master->line_read = readline("\033[32mminishell:~$ \033[0m");
		if (!master->line_read)
			return (handle_eof(master), EXIT_SUCCESS);
		master->line_count++;
		if (*master->line_read)
			add_history(master->line_read);
		launch_expansion_main(master);
		if (launch_lexer(master, master->line_read, &master->token)
			== EXIT_SUCCESS)
			launch_execution(master);
		free(master->exec);
		free_token(&(master->token));
		free(master->line_read);
		free(master->pids);
	}
}

int	main(void)
{
	t_master	master;

	rl_catch_signals = 0;
	master.env_list = NULL;
	master.exit_status = 0;
	master.pids = NULL;
	master.prev_exit_status = 0;
	master.line_count = 0;
	g_exit_status = &master.prev_exit_status;
	set_sigaction(&master);
	manage_environment(&master, &master.env_list);
	return (shell_loop(&master));
}
