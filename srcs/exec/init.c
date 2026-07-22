/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 00:52:36 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:26 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	load_texture(t_game *game, t_texture *tex, char *path)
{
	tex->img_ptr = mlx_xpm_file_to_image(game->mlx, path,
			&tex->width, &tex->height);
	if (!tex->img_ptr)
		error_exit(game, "Texture loading failed.");
	tex->data = mlx_get_data_addr(tex->img_ptr, &tex->bpp,
			&tex->line_len, &tex->endian);
}

void	load_all_textures(t_game *game)
{
	load_texture(game, &game->tex_n, game->no_path);
	load_texture(game, &game->tex_s, game->so_path);
	load_texture(game, &game->tex_e, game->ea_path);
	load_texture(game, &game->tex_w, game->we_path);
}

void	init_game(t_game *game, char *map_file)
{
	game->fd = -1;
	game->floor_color = -1;
	game->ceiling_color = -1;
	parse_cub_file(map_file, game);
	game->mlx = mlx_init();
	if (!game->mlx)
		error_exit(game, "MLX init failed.");
	load_all_textures(game);
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp,
			&game->size_line, &game->endian);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
