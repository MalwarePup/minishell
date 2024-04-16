/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:32:49 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/16 15:09:39 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "libft.h"
#include "minishell.h"

int	count_pipe(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->type == CMD_PIPE)
			i++;
		token = token->next;
	}
	return (i + 1);
}

void	execution(t_master *master, t_token *token, t_cmd_type type)
{
	launch_redirection(master, token->redir);
	if (type == CMD_OTHERS)
		execute_command(master);
	else
		master->exit_status = execute_builtin(master, type);
}
