/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:34:31 by ladloff           #+#    #+#             */
/*   Updated: 2024/02/17 11:45:20 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

static char	*create_new_string_with_value(t_master *master,
	t_expansion *exp, char **str)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(*str) + ft_strlen((*exp).value) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		free((*exp).value);
		ft_error_exit(master, "malloc (create_new_string_with_value)", ENOMEM,
			true);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, *str,
		(*exp).substr_start - *str + 1);
	ft_strlcat(new_str, (*exp).value, len);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static char	*create_new_string_without_value(t_master *master,
	t_expansion *exp, char **str)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(*str) + 1;
	new_str = malloc(len);
	if (!new_str)
	{
		free((*exp).name);
		ft_error_exit(master, "malloc (create_new_string_with_value)", ENOMEM,
			true);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(new_str, *str,
		(*exp).substr_start - *str + 1);
	ft_strlcat(new_str, (*exp).substr_start + ft_strlen((*exp).name) + 1,
		len);
	return (new_str);
}

static void	process_expansion_replace(t_master *master, char **str,
	t_expansion *exp)
{
	char	*new_str;

	if ((*exp).value)
		new_str = create_new_string_with_value(master, exp, str);
	else
		new_str = create_new_string_without_value(master, exp, str);
	free(*str);
	*str = new_str;
}

static void	process_expansion(t_master *master,
	t_expansion *exp, char **str, size_t *i)
{
	exp->substr_start = *str + *i;
	if (*str[*i + 1] == '?')
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
	process_expansion_replace(master, str, exp);
	free((*exp).name);
	free((*exp).value);
}

void	launch_expansion(t_master *master, char *str)
{
	char		quote;
	size_t		i;
	t_expansion	exp;

	i = 0;
	quote = 0;
	if (!(str))
		return ;
	while (str[i])
	{
		condition_while(str, i, true, &quote);
		if (str[i] == '$' && quote != '\'' && (ft_isalpha(str[i + 1])
				|| str[i + 1] == '?')
				&& (i == 0 || (i > 0 && str[i - 1] != '$')))
        process_expansion(master, &exp, &str, &i);
		else
			i++;
	}
}
