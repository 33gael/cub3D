/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 00:53:36 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 16:34:31 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	touch(float px, float py, t_game *game)
{
	int	x;
	int	y;

	x = px / BLOCK;
	y = py / BLOCK;
	if (game->map[y][x] == '1')
		return (true);
	return (false);
}

int	get_texture_pixel(t_texture *tex, int x, int y)
{
	unsigned char	*pixel;
	unsigned int	color;
	int				b;
	int				shift;

	if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
		return (0);
	pixel = (unsigned char *)tex->data + y * tex->line_len
		+ x * (tex->bpp / 8);
	if (tex->bpp == 32 && !tex->endian)
		return (*(unsigned int *)pixel & 0xffffff);
	color = 0;
	b = -1;
	while (++b < tex->bpp / 8)
	{
		shift = b * 8;
		if (tex->endian)
			shift = (tex->bpp / 8 - b - 1) * 8;
		color |= (unsigned int)pixel[b] << shift;
	}
	return (color & 0xffffff);
}

static int	blend_color(unsigned int a, unsigned int b, float weight)
{
	unsigned int	w;
	unsigned int	rb;
	unsigned int	green;

	w = weight * 256 + 0.5f;
	rb = ((a & 0xff00ff) * (256 - w)
			+ (b & 0xff00ff) * w + 0x800080) >> 8;
	green = ((a & 0x00ff00) * (256 - w)
			+ (b & 0x00ff00) * w + 0x8000) >> 8;
	return ((rb & 0xff00ff) | (green & 0x00ff00));
}

int	sample_texture(t_texture *tex, float x, float y)
{
	int	x1;
	int	y1;
	int	top;
	int	bottom;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x > tex->width - 1)
		x = tex->width - 1;
	if (y > tex->height - 1)
		y = tex->height - 1;
	x1 = (int)x + (x < tex->width - 1);
	y1 = (int)y + (y < tex->height - 1);
	top = blend_color(get_texture_pixel(tex, (int)x, (int)y),
			get_texture_pixel(tex, x1, (int)y), x - (int)x);
	bottom = blend_color(get_texture_pixel(tex, (int)x, y1),
			get_texture_pixel(tex, x1, y1), x - (int)x);
	return (blend_color(top, bottom, y - (int)y));
}
