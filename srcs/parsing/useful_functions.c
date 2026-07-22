/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useful_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 09:16:40 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:46 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	error_exit(t_game *game, char *message)
{
	printf("Error\n%s\n", message);
	free_game(game);
	exit(EXIT_FAILURE);
}

void	free_map(char **map)
{
	int	i;

	i = 0;
	if (!map)
		return ;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

char	**duplicate_map(char **map, int height)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (height + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < height)
	{
		copy[i] = ft_strdup(map[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	free_mlx(t_game *g)
{
	if (g->tex_n.img_ptr)
		mlx_destroy_image(g->mlx, g->tex_n.img_ptr);
	if (g->tex_s.img_ptr)
		mlx_destroy_image(g->mlx, g->tex_s.img_ptr);
	if (g->tex_e.img_ptr)
		mlx_destroy_image(g->mlx, g->tex_e.img_ptr);
	if (g->tex_w.img_ptr)
		mlx_destroy_image(g->mlx, g->tex_w.img_ptr);
	if (g->img)
		mlx_destroy_image(g->mlx, g->img);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
}

void	free_game(t_game *g)
{
	char	*trash;

	if (!g)
		return ;
	free_map(g->map);
	free(g->no_path);
	free(g->so_path);
	free(g->we_path);
	free(g->ea_path);
	free(g->map_buffer);
	free(g->current_line);
	if (g->fd >= 0)
	{
		trash = get_next_line(g->fd);
		while (trash)
		{
			free(trash);
			trash = get_next_line(g->fd);
		}
		close(g->fd);
	}
	free_mlx(g);
}
