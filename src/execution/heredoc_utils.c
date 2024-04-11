/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:59:43 by ladloff           #+#    #+#             */
/*   Updated: 2024/04/11 12:37:21 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "libft.h"
#include "minishell.h"

static char	*create_filename(int index)
{
	char	*itoa;
	char	*filename;

	itoa = ft_itoa(index);
	if (!itoa)
		return (NULL);
	filename = ft_strjoin3("/tmp/heredoc_", itoa);
	return (filename);
}

static int	file_exists_with_permissions(const char *filename)
{
	return (access(filename, R_OK | W_OK) == 0);
}

int	create_or_open_file(char **filename, int *index)
{
	struct stat	buf;
	int			fd;

	fd = -1;
	while (*filename == NULL || stat(*filename, &buf) == 0)
	{
		if (*filename != NULL && file_exists_with_permissions(*filename))
		{
			fd = open(*filename, O_WRONLY, 0644);
			break ;
		}
		free(*filename);
		*filename = create_filename(++(*index));
		if (*filename == NULL)
			return (-1);
		fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd != -1)
			break ;
	}
	return (fd);
}
