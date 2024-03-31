/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_quotes_removal.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:59:16 by ladloff           #+#    #+#             */
/*   Updated: 2024/03/31 17:25:32 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

// Fix a segfault when the argument is NULL, but the error management is not
// consistent with the rest of the project.
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

// typedef struct s_arg
// {
// 	char	*new_arg;
// 	size_t	current_size;
// 	size_t	new_arg_capacity;
// }				t_arg;

// static char	*ft_strjoin_char(char **str, char c, size_t *capacity,
// 		size_t *current_size)
// {
// 	size_t	len;
// 	size_t	new_capacity;
// 	char	*new_str;

// 	len = ft_strlen(*str);
// 	if (len + 2 > *capacity)
// 	{
// 		new_capacity = *capacity * 2;
// 		new_str = ft_realloc(*str, *current_size, new_capacity);
// 		if (!new_str)
// 		{
// 			free(*str);
// 			return (NULL);
// 		}
// 		*str = new_str;
// 		*capacity = new_capacity;
// 	}
// 	(*str)[len] = c;
// 	(*str)[len + 1] = '\0';
// 	*current_size = len + 2;
// 	return (*str);
// }

// static void	handle_quotes(char *arg, int *j, t_arg *arg_processing)
// {
// 	char	quote_type;

// 	quote_type = arg[*j];
// 	while (arg[++(*j)] && arg[*j] != quote_type)
// 	{
// 		if (!ft_strjoin_char(&(arg_processing->new_arg), arg[*j],
// 				&(arg_processing->new_arg_capacity),
// 				&(arg_processing->current_size)))
// 			break ;
// 	}
// }

// static char	*process_arg(char *arg)
// {
// 	int				j;
// 	t_arg	arg_processing;

// 	arg_processing.current_size = 0;
// 	arg_processing.new_arg_capacity = 64;
// 	arg_processing.new_arg = malloc(arg_processing.new_arg_capacity);
// 	if (!arg_processing.new_arg)
// 		return (NULL);
// 	arg_processing.new_arg[0] = '\0';
// 	j = 0;
// 	while (arg[j])
// 	{
// 		if ((arg[j] == '"' || arg[j] == '\''))
// 			handle_quotes(arg, &j, &arg_processing);
// 		else
// 			if (!ft_strjoin_char(&(arg_processing.new_arg), arg[j],
// 					&(arg_processing.new_arg_capacity),
// 					&(arg_processing.current_size)))
// 				break ;
// 		j++;
// 	}
// 	return (arg_processing.new_arg);
// }

// void	replace_argv_without_quotes(t_master *master)
// {
// 	int		i;
// 	char	*new_arg;

// 	i = 0;
// 	while (master->argv[i])
// 	{
// 		new_arg = process_arg(master->argv[i]);
// 		if (new_arg)
// 		{
// 			free(master->argv[i]);
// 			master->argv[i] = new_arg;
// 		}
// 		i++;
// 	}
// }
