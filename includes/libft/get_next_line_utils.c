/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbosvie <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 08:41:55 by thbosvie          #+#    #+#             */
/*   Updated: 2025/12/01 13:03:06 by thbosvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

char	*ft_strdup_mod(const char *s, int len)
{
	int		i;
	char	*str;

	i = 0;
	while (s[i])
		i++;
	if (len == 0)
		str = malloc(sizeof(char) * (i + 1));
	else
		str = malloc(sizeof(char) * (i + len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin_mod(char *s1, char *s2)
{
	char	*res;
	size_t	i;

	i = 0;
	if (!s2)
		return (NULL);
	if (s1)
	{
		res = ft_strdup_mod(s1, ft_strlen(s2));
		if (!res)
		{
			free(s1);
			return (NULL);
		}
		while (i < ft_strlen(s2))
		{
			res[ft_strlen(s1) + i] = s2[i];
			i++;
		}
		res[ft_strlen(s1) + i] = '\0';
		free(s1);
	}
	else
		res = ft_strdup_mod(s2, 0);
	return (res);
}

char	*ft_substr_mod(char const *start, char const *end)
{
	char	*str;
	int		i;

	i = 0;
	if (!start || !end)
		return (NULL);
	str = malloc(sizeof(char) * (end - start + 1));
	if (!str)
		return (NULL);
	while (start != end)
	{
		str[i] = *start;
		i++;
		start++;
	}
	str[i] = '\0';
	return (str);
}
