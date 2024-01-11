/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:18:59 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 18:38:30 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

void	cleanup_executable(t_master *master)
{
	free_double_ptr(master->exec->argv);
	free(master->exec->pathname);
	free(master->exec);
}

void	cleanup_before_exit(t_master *master)
{
	rl_clear_history();
	free_environment_list(master->env_list);
	free_token_list(master->token_list);
	free(master->line_read);
}
