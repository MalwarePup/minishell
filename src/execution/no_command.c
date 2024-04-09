/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 10:51:38 by macbookpro        #+#    #+#             */
/*   Updated: 2024/04/09 11:20:49 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

int handle_redir(t_master *master, t_token *token, bool *is_input, bool *is_output)
{
    int exit_status;

    exit_status = 2;
    replace_redir(master, &token->data);
    if (token->type == CMD_RED_IN || token->type == CMD_D_RED_IN)
    {
        if (token->type == CMD_RED_IN)
            exit_status = redirect_cmd(master, token->data, O_RDONLY, STDIN_FILENO);
        else if (token->type == CMD_D_RED_IN)
            exit_status = redirect_cmd(master, token->data, O_RDONLY, STDIN_FILENO);
        if (exit_status == true)
            *is_input = true;
    }
    else if (token->type == CMD_RED_OUT || token->type == CMD_D_RED_OUT)
    {
        if (token->type == CMD_RED_OUT)
            exit_status = redirect_cmd(master, token->data, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
        else if (token->type == CMD_D_RED_OUT)
            exit_status = redirect_cmd(master, token->data, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
        if (exit_status == true)
            *is_output = true;
    }
    if (exit_status == false)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}