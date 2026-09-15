/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 00:52:57 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:24 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	index = y * game->size_line + x * game->bpp / 8;
	if (game->bpp == 32 && !game->endian)
	{
		*(unsigned int *)(game->data + index) = color;
		return ;
	}
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

void	clear_image(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}

void	draw_square(t_point p, int size, int color, t_game *game)
{
	int	i;

	i = -1;
	while (++i < size)
		put_pixel(p.x + i, p.y, color, game);
	i = -1;
	while (++i < size)
		put_pixel(p.x, p.y + i, color, game);
	i = -1;
	while (++i < size)
		put_pixel(p.x + size, p.y + i, color, game);
	i = -1;
	while (++i < size)
		put_pixel(p.x + i, p.y + size, color, game);
}

void	draw_map(t_game *game)
{
	t_point	p;
	int		y;
	int		x;

	y = -1;
	while (game->map[++y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == '1')
			{
				p.x = x * BLOCK;
				p.y = y * BLOCK;
				draw_square(p, BLOCK, 0x0000FF, game);
			}
			x++;
		}
	}
}

void	draw_3d_dda(int x, int start_y, int end_y, t_game *mapa)
{
	int	y;

	y = -1;
	while (++y < start_y)
		put_pixel(x, y, mapa->ceiling_color, mapa);
	y = end_y;
	while (y < HEIGHT)
	{
		put_pixel(x, y, mapa->floor_color, mapa);
		y++;
	}
}
