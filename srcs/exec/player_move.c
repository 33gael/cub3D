/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 00:53:57 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:32 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	blocked(t_game *g, float x, float y)
{
	return (touch(x - 12, y - 12, g) || touch(x + 12, y - 12, g)
		|| touch(x - 12, y + 12, g) || touch(x + 12, y + 12, g));
}

static void	movement(t_game *g, float *dx, float *dy)
{
	float	forward;
	float	side;
	float	speed;

	forward = g->player.key_up - g->player.key_down;
	side = g->player.key_right - g->player.key_left;
	speed = 160 * g->delta_time;
	if (forward && side)
		speed /= sqrtf(2);
	*dx = (cosf(g->player.angle) * forward
			- sinf(g->player.angle) * side) * speed;
	*dy = (sinf(g->player.angle) * forward
			+ cosf(g->player.angle) * side) * speed;
}

void	move_player(t_game *g)
{
	float	dx;
	float	dy;

	if (!g->focused)
		return ;
	g->player.angle += (g->player.right_rotate - g->player.left_rotate)
		* 2.2 * g->delta_time;
	g->player.angle = remainderf(g->player.angle, 2 * PI);
	movement(g, &dx, &dy);
	if (!blocked(g, g->player.x + dx, g->player.y))
		g->player.x += dx;
	if (!blocked(g, g->player.x, g->player.y + dy))
		g->player.y += dy;
}
