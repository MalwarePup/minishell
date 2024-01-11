/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 21:46:41 by ladloff           #+#    #+#             */
/*   Updated: 2024/01/11 21:46:59 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_escaped(const char *str, int index)
{
	int	backslashes;

	backslashes = 0;
	while (index > 0 && str[index - 1] == '\\')
	{
		backslashes++;
		index--;
	}
	return (backslashes % 2 != 0);
}
