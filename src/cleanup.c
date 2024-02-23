/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 18:18:59 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/23 15:37:00 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

void	free_string_array(char ***str)
{
	char	**ptr;

	ptr = *str;
	if (!ptr)
		return ;
	while (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
		ptr++;
	}
	free(*str);
	*str = NULL;
}

void	free_token(t_token **token)
{
	t_token	*next;
	t_token	*current;

	current = *token;
	while (current)
	{
		next = current->next;
		free(current->data);
		if (current->redir)
			free_token(&current->redir);
		free(current);
		current = next;
	}
	*token = NULL;
}

void	cleanup_before_exit(t_master *master)
{
	rl_clear_history();
	free(master->pid_list);
	free_string_array(&master->argv);
	free_environment_list(&master->env_list);
	free_token(&master->token);
	free(master->line_read);
	free(master->exec);
}

void	clean_lexer(t_lexer *lexer)
{
	free_token(&lexer->redirect);
	free(lexer->data_command);
	lexer->data_command = NULL;
	free(lexer->data_redir);
	lexer->data_redir = NULL;
}
