/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 09:22:23 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:35 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

int	parse_color(char *line, t_game *game)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;

	line++;
	while (*line == ' ')
		line++;
	rgb = ft_split(line, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
	{
		if (rgb)
			free_map(rgb);
		error_exit(game, "Invalid colors format (RGB)");
	}
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	free_map(rgb);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		error_exit(game, "RGB limits disrespected");
	return (get_rgb(r, g, b));
}
