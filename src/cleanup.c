/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:18:59 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/12 16:59:00 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

void	free_string_array(char **str)
{
	char	**ptr;

	if (!str)
		return ;
	ptr = str;
	while (*ptr)
		free(*ptr++);
	free(str);
}

void	cleanup_executable(t_master *master)
{
	free_string_array(master->exec->argv);
	free(master->exec->pathname);
	master->exec->pathname = NULL;
}

void	cleanup_before_exit(t_master *master)
{
	rl_clear_history();
	free_environment_list(master->env_list);
	free_token(&(master->token));
	free(master->line_read);
	free(master->exec);
}
