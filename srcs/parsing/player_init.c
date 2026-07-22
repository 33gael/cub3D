/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 00:56:00 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:40 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_player_dir(t_player *p, char dir)
{
	if (dir == 'N')
		p->angle = 3 * PI / 2;
	else if (dir == 'S')
		p->angle = PI / 2;
	else if (dir == 'E')
		p->angle = 0;
	else if (dir == 'W')
		p->angle = PI;
	p->key_up = false;
	p->key_down = false;
	p->key_left = false;
	p->key_right = false;
	p->left_rotate = false;
	p->right_rotate = false;
}

static void	extract_player(t_game *g, int x, int y, int *count)
{
	char	c;

	c = g->map[y][x];
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		g->player.x = (x * BLOCK) + (BLOCK / 2);
		g->player.y = (y * BLOCK) + (BLOCK / 2);
		set_player_dir(&g->player, c);
		g->map[y][x] = '0';
		(*count)++;
	}
}

void	init_player(t_game *game)
{
	int	y;
	int	x;
	int	count;

	count = 0;
	y = -1;
	while (game->map[++y])
	{
		x = -1;
		while (game->map[y][++x])
			extract_player(game, x, y, &count);
	}
	if (count != 1)
		error_exit(game, "Need exactly one player (N, S, E, W)");
}
