/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:37:32 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:37 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	flood_fill(char **map, int x, int y, int max_y)
{
	if (y < 0 || y >= max_y || x < 0 || !map[y])
		return (0);
	if (x >= (int)ft_strlen(map[y]) || map[y][x] == ' ' || map[y][x] == '\0')
		return (0);
	if (map[y][x] == '1' || map[y][x] == 'V')
		return (1);
	map[y][x] = 'V';
	if (!flood_fill(map, x + 1, y, max_y) || !flood_fill(map, x - 1, y, max_y)
		|| !flood_fill(map, x, y + 1, max_y) || !flood_fill(map, x, y - 1,
			max_y))
		return (0);
	if (!flood_fill(map, x + 1, y + 1, max_y) || !flood_fill(map, x - 1, y - 1,
			max_y) || !flood_fill(map, x + 1, y - 1, max_y) || !flood_fill(map,
			x - 1, y + 1, max_y))
		return (0);
	return (1);
}

void	check_map_closed(t_game *game)
{
	char	**map_copy;
	int		start_x;
	int		start_y;

	map_copy = duplicate_map(game->map, game->y);
	if (!map_copy)
		error_exit(game, "Error of map copy");
	start_x = (int)(game->player.x / BLOCK);
	start_y = (int)(game->player.y / BLOCK);
	if (flood_fill(map_copy, start_x, start_y, game->y) == 0)
	{
		free_map(map_copy);
		error_exit(game, "You didn't closed the map\n");
	}
	free_map(map_copy);
}

void	check_map_characters(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] != '0' && game->map[y][x] != '1'
				&& game->map[y][x] != 'N' && game->map[y][x] != 'S'
				&& game->map[y][x] != 'E' && game->map[y][x] != 'W'
				&& game->map[y][x] != ' ')
			{
				error_exit(game, "Invalid character found in the map file.");
			}
			x++;
		}
		y++;
	}
}
