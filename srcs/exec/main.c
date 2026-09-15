/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 17:47:16 by thbosvie          #+#    #+#             */
/*   Updated: 2026/07/17 15:19:28 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	close_game(t_game *game)
{
	free_game(game);
	exit(0);
	return (0);
}

int	draw_loop(t_game *game)
{
	float	start_x;
	int		i;
	t_point	p;

	frame_time(game);
	move_player(game);
	if (DEBUG)
	{
		p.x = game->player.x;
		p.y = game->player.y;
		draw_square(p, 10, 0x00FF00, game);
		draw_map(game);
	}
	i = 0;
	while (i < WIDTH)
	{
		start_x = game->player.angle + atan((2.0 * (i + 0.5)
				/ WIDTH - 1.0) * tan(PI / 6));
		draw_line(&game->player, game, start_x, i);
		i++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		printf("Error\nUsage: ./cub3D <map.cub>\n");
		return (1);
	}
	ft_bzero(&game, sizeof(t_game));
	init_game(&game, argv[1]);
	init_timing(&game);
	mlx_hook(game.win, 17, 0L, close_game, &game);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	return (0);
}
