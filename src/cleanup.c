/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:18:59 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/09 23:13:20 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "exit.h"
#include "env.h"
#include "execution.h"

void	cleanup_executable(void)
{
	free_double_ptr(g_master.exec->argv);
	free(g_master.exec->pathname);
	free(g_master.exec);
}

void	cleanup_before_exit(void)
{
	rl_clear_history();
	free_environment_list(g_master.env_list);
	free_token_list(g_master.token_list);
	free(g_master.line_read);
}
