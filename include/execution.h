/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 12:12:59 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 11:56:57 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdbool.h>
# include "minishell.h"
# include "lexer.h"

# define DEFAULT_PATH_1 "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin"
# define DEFAULT_PATH_2 ":/opt/homebrew/bin"

typedef enum e_builtin_type
{
	T_ERROR = 1,
	T_CD,
	T_ECHO,
	T_ENV,
	T_EXIT,
	T_PWD,
	T_UNSET,
	T_EXPORT,
	T_OTHERS,
}	t_builtin_type;

/* builtin_utils.c */
void			print_data_builtins(t_exec	*current);
void			handle_error_cases(t_master *master, t_exec *exec);

/* builtin.c */
t_builtin_type	execute_command_or_builtin(t_master *master, t_exec *exec);

/* execution_mem.c */
void			free_double_ptr(char **str);
t_exec			*create_arguments(t_token *token);

/* execution_utils.c */
char			**env_list_to_array(t_env *env_list);

/* execution.c */
void			launch_execution(t_master *master);

/* expansion_utils.c */
char			*get_env_value(t_env *env, char *name);
char			*extract_expansion_name(char *str);

/* expansion.c */
void			launch_expansion(t_master *master, t_exec *exec);

/* split_args.c */
void			split_args(char *s, char **argv, bool *simple_quotes);

#endif /* EXECUTION_H */
