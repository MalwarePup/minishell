/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:17:55 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/05 11:04:12 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

# define SHLVL "SHLVL"

/* env_utils.c */

void	update_executable_path(t_exec *exec, t_env *env_list);
void	print_environement_list(t_env *env_list);
void	print_environement_var(t_env *env_list, char *name);
char	*update_shlvl(char *value, char *name);

/* env.c */

void	free_environment_list(t_env *env);
void	manage_environment(t_env **env_list);

#endif /* ENV_H */
