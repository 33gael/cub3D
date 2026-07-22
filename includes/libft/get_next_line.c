/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbosvie <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 08:41:25 by thbosvie          #+#    #+#             */
/*   Updated: 2025/11/27 17:32:35 by thbosvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static char	*split_buff(char **buff, char *chr)
{
	char	*line;
	char	*tmp;

	line = ft_substr_mod(*buff, chr + 1);
	if (!line)
		return (NULL);
	if (ft_strlen(chr) > 1)
	{
		tmp = ft_strdup_mod(chr + 1, 0);
		free(*buff);
		*buff = tmp;
	}
	else
	{
		free(*buff);
		*buff = NULL;
	}
	return (line);
}

static char	*fill_buff(int fd, int *readed, char **buff, char **chr)
{
	char	*tmp;

	tmp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!tmp)
		return (NULL);
	*readed = read(fd, tmp, BUFFER_SIZE);
	if (*readed < 0)
	{
		free(tmp);
		return (NULL);
	}
	tmp[*readed] = '\0';
	if (*readed > 0)
		*buff = ft_strjoin_mod(*buff, tmp);
	free(tmp);
	if (!*buff)
		return (NULL);
	*chr = ft_strchr(*buff, '\n');
	return (*buff);
}

static char	*read_line(int fd, char **buff)
{
	int		readed;
	char	*chr;

	readed = BUFFER_SIZE;
	chr = NULL;
	while (readed == BUFFER_SIZE && !chr)
	{
		*buff = fill_buff(fd, &readed, buff, &chr);
		if (!*buff)
			return (NULL);
	}
	if ((readed != BUFFER_SIZE && !chr) || (readed == BUFFER_SIZE && !chr))
		return (*buff);
	if (readed == 0)
		return (NULL);
	return (split_buff(buff, chr));
}

char	*get_next_line(int fd)
{
	static char	*buff;
	char		*line;
	char		*chr;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (buff)
	{
		chr = ft_strchr(buff, '\n');
		if (chr)
		{
			line = split_buff(&buff, chr);
			return (line);
		}
	}
	line = read_line(fd, &buff);
	if (line == buff && line != NULL)
	{
		line = ft_strdup_mod(buff, 0);
		free(buff);
		buff = NULL;
	}
	if (line == NULL && buff)
		free(buff);
	return (line);
}
