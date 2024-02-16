/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:34:31 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/15 18:16:24 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

static char	*create_new_string_with_value(t_master *master, t_exec *exec,
	t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(exp->redir->data) + ft_strlen((*exp).value) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		free((*exp).value);
		ft_error_exit(master, "malloc (create_new_string_with_value)", ENOMEM,
			true);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, exp->redir->data,
		(*exp).substr_start - exp->redir->data + 1);
	ft_strlcat(new_str, (*exp).value, len);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static char	*create_new_string_without_value(t_master *master,
	t_exec *exec, t_expansion *exp)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(exp->redir->data) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		ft_error_exit(master, "malloc (create_new_string_with_value)", ENOMEM,
			true);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, exp->redir->data,
		(*exp).substr_start - exp->redir->data + 1);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static void	process_expansion_replace(t_master *master, t_exec *exec,
	t_expansion *exp)
{
	char	*new_str;

	if ((*exp).value)
		new_str = create_new_string_with_value(master, exec, exp);
	else
		new_str = create_new_string_without_value(master, exec, exp);
	free(exp->redir->data);
	exp->redir->data = new_str;
}

static void	process_expansion(t_master *master, t_exec *exec, t_expansion *exp)
{
	exp->substr_start = exp->redir->data + exp->j;
	if (exp->redir->data[(*exp).j + 1] == '?')
	{
		(*exp).name = extract_expansion_name(master, (*exp).substr_start);
		(*exp).value = ft_itoa(master->prev_exit_status);
		if (!(*exp).value)
		{
			free((*exp).name);
			ft_error_exit(master, "ft_itoa (process_expansion)", ENOMEM, true);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		(*exp).name = extract_expansion_name(master, (*exp).substr_start);
		(*exp).value = get_env_value(master, master->env_list, (*exp).name);
	}
	process_expansion_replace(master, exec, exp);
	free((*exp).name);
	free((*exp).value);
}

void	launch_expansion_redirc(t_master *master)
{
	t_expansion	exp;
	char		quote;

	quote = 0;
	while (exp.current)
	{
    exp.redir = exp.current->redir;
    while (exp.redir)
    {
      while (exp.redir->data[exp.j])
      {
		    exp.j = 0;
        condition_while(exp.redir->data, exp.j, true, &quote);
        if (exp.redir->data[exp.j] == '$' && quote != '\''
          && (ft_isalpha(exp.redir->data[exp.j + 1])
          || exp.redir->data[exp.j + 1] == '?') && (exp.j == 0
          || (exp.j > 0 && exp.redir->data[exp.j - 1] != '$')))
          process_expansion(master, master->exec, &exp);
        else
          exp.j++;
      }
      replace_redir_without_quotes(exp.redir->data);
      exp.redir = exp.redir->next;
    }
    exp.current = exp.current->next;
  }
}
