/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_quotes_removal.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:59:16 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/08 10:11:55 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

static char	*ft_strjoin_char(char *str, char c)
{
	int		len;
	char	*new_str;
	int		i;

	len = ft_strlen(str);
	new_str = malloc((len + 2));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str);
	return (new_str);
}

static char	*process_arg(char *arg)
{
	char	*new_arg;
	char	quote_type;
	int		j;

	new_arg = ft_strdup("");
	if (!new_arg)
		return (NULL);
	j = -1;
	while (arg[++j])
	{
		if (arg[j] == '"' || arg[j] == '\'')
		{
			quote_type = arg[j];
			while (arg[++j] && arg[j] != quote_type)
				new_arg = ft_strjoin_char(new_arg, arg[j]);
		}
		else
		{
			new_arg = ft_strjoin_char(new_arg, arg[j]);
			if (!new_arg)
				return (NULL);
		}
	}
	return (new_arg);
}

void	replace_argv_without_quotes(t_master *master)
{
	int		i;
	char	*new_arg;

	i = -1;
	while (master->argv[++i])
	{
		new_arg = process_arg(master->argv[i]);
		if (!new_arg)
			return ;
		free(master->argv[i]);
		master->argv[i] = new_arg;
	}
}
