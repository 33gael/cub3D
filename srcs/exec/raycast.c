/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 00:53:13 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 18:29:40 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_dda(t_player *p, t_ray *r, float angle)
{
	r->ray_x = cos(angle);
	r->ray_y = sin(angle);
	r->map_x = (int)(p->x / BLOCK);
	r->map_y = (int)(p->y / BLOCK);
	r->delta_dx = fabs(1.0f / r->ray_x);
	r->delta_dy = fabs(1.0f / r->ray_y);
	r->step_x = 1;
	r->side_dx = (r->map_x + 1.0f - (p->x / BLOCK)) * r->delta_dx;
	if (r->ray_x < 0)
	{
		r->step_x = -1;
		r->side_dx = ((p->x / BLOCK) - r->map_x) * r->delta_dx;
	}
	r->step_y = 1;
	r->side_dy = (r->map_y + 1.0f - (p->y / BLOCK)) * r->delta_dy;
	if (r->ray_y < 0)
	{
		r->step_y = -1;
		r->side_dy = ((p->y / BLOCK) - r->map_y) * r->delta_dy;
	}
}

static void	perform_dda(t_game *g, t_ray *r)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (r->side_dx < r->side_dy)
		{
			r->side_dx += r->delta_dx;
			r->map_x += r->step_x;
			r->side = 0;
		}
		else
		{
			r->side_dy += r->delta_dy;
			r->map_y += r->step_y;
			r->side = 1;
		}
		if (g->map[r->map_y][r->map_x] == '1')
			hit = 1;
	}
}

static void	get_wall_texture(t_game *g, t_ray *r, t_player *p)
{
	if (r->side == 0)
	{
		r->dist = (r->side_dx - r->delta_dx);
		r->wall_x = (p->y / BLOCK) + r->dist * r->ray_y;
		r->tex = &g->tex_e;
		if (r->ray_x > 0)
			r->tex = &g->tex_w;
	}
	else
	{
		r->dist = (r->side_dy - r->delta_dy);
		r->wall_x = (p->x / BLOCK) + r->dist * r->ray_x;
		r->tex = &g->tex_s;
		if (r->ray_y > 0)
			r->tex = &g->tex_n;
	}
	r->wall_x -= floor(r->wall_x);
	if ((r->side == 0 && r->ray_x < 0) || (r->side == 1 && r->ray_y > 0))
		r->wall_x = 1.0f - r->wall_x;
}

static void	draw_textured_line(t_game *g, t_ray *r, int i)
{
	int		y;
	float	tex_y;
	float	step;
	float	tex_x;
	int		color;

	y = fmaxf(0, r->start_y);
	step = r->tex->height / r->height;
	tex_y = (y + 0.5f - (HEIGHT - r->height) * 0.5f) * step;
	tex_x = fminf(r->tex->width - 1, r->wall_x * r->tex->width);
	while (y < r->end && y < HEIGHT)
	{
		if (step < 1)
			color = sample_texture(r->tex, tex_x - 0.5f, tex_y - 0.5f);
		else
			color = get_texture_pixel(r->tex, (int)tex_x, (int)tex_y);
		put_pixel(i, y, color, g);
		tex_y += step;
		y++;
	}
}

void	draw_line(t_player *p, t_game *g, float start_x, int i)
{
	t_ray	r;

	init_dda(p, &r, start_x);
	perform_dda(g, &r);
	get_wall_texture(g, &r, p);
	r.dist *= BLOCK;
	r.dist *= cos(start_x - p->angle);
	if (r.dist < 0.0001f)
		r.dist = 0.0001f;
	r.height = (BLOCK / r.dist) * (WIDTH / (2.0f * tan(PI / 6)));
	r.start_y = (HEIGHT - r.height) / 2;
	r.end = r.start_y + r.height;
	draw_3d_dda(i, r.start_y, r.end, g);
	draw_textured_line(g, &r, i);
}
