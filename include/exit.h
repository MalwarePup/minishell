/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:35:15 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/18 17:06:35 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H

# include <errno.h>
# include "minishell.h"

# define ERR_PREFIX "\n\nError: "
# define ENV_ERR "the env is empty"
# define MALLOC_ERR "during memory allocation"
# define SIGACTION_ERR "during setting signal handler"

extern t_master	g_master;

/* cleanup.c */
void	cleanup_before_exit(void);
void	cleanup_executable(void);

/* exit.c */
void	error_exit(char *error_str);
void	ft_error_exit(char *error_str, int errnum);
void	handle_eof(void);

#endif /* EXIT_H */
