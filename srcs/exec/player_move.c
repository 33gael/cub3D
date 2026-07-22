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

static void	rotate_player(t_player *p)
{
	if (p->left_rotate)
		p->angle -= 0.03;
	if (p->right_rotate)
		p->angle += 0.03;
	if (p->angle > 2 * PI)
		p->angle = 0;
	if (p->angle < 0)
		p->angle = 2 * PI;
}

static void	apply_movement(t_player *p, float *n, float ca, float sa)
{
	if (p->key_up)
	{
		n[0] += ca;
		n[1] += sa;
	}
	if (p->key_down)
	{
		n[0] -= ca;
		n[1] -= sa;
	}
	if (p->key_left)
	{
		n[1] -= ca;
		n[0] += sa;
	}
	if (p->key_right)
	{
		n[1] += ca;
		n[0] -= sa;
	}
}

void	move_player(t_game *g)
{
	float	ca;
	float	sa;
	float	n[2];

	rotate_player(&g->player);
	ca = cos(g->player.angle) * 3;
	sa = sin(g->player.angle) * 3;
	n[0] = g->player.x;
	n[1] = g->player.y;
	apply_movement(&g->player, n, ca, sa);
	if (!touch(n[0], g->player.y, g))
		g->player.x = n[0];
	if (!touch(g->player.x, n[1], g))
		g->player.y = n[1];
}
