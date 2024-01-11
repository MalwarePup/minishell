/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 11:34:21 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 07:23:36 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

typedef struct s_master	t_master;

int		ft_cd(int argc, char **argv, t_master *master);
int		ft_echo(int argc, char **argv);
void	ft_env(t_master *master);
void	ft_exit(int argc, char **argv);
int		ft_export(int argc, char **argv, t_master *master);
int		ft_pwd(void);
int		ft_unset(int argc, char **argv, t_master *master);

#endif /* BUILTINS_H */
