/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:47 by chmadran          #+#    #+#             */
/*   Updated: 2024/01/09 15:23:26 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <sys/types.h>
# include "lexer.h"

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*last;
}	t_env;

typedef struct s_exec
{
	int			argc;
	char		**argv;
	char		*pathname;
	t_token		*token;
	pid_t		pid;
	int			pipefd[2];
	int			old_pipefd[2];
	bool		first_cmd;
	bool		simple_quotes;
}	t_exec;

typedef struct s_master
{
	t_env	*env_list;
	char	*line_read;
	t_token	*token_list;
	t_exec	*exec;
	int		exit_status;
}	t_master;

#endif /* MINISHELL_H */
