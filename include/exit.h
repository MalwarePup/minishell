/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:35:15 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 09:06:55 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H

# include <errno.h>
# include "minishell.h"

# define NUM_ARG_ERR "minishell: exit: %s: numeric argument required\n"
# define TOO_MANY_ARGS_ERR "minishell: exit: too many arguments\n"

extern t_master	g_master;

/* cleanup.c */
void	cleanup_before_exit(void);
void	cleanup_executable(void);

/* exit.c */
void	error_exit(char *error_str);
void	ft_error_exit(char *error_str, int errnum);
void	handle_eof(void);

#endif /* EXIT_H */
