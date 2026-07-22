/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 09:25:35 by gaeducas            #+#    #+#             */
/*   Updated: 2026/07/17 15:19:47 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*get_path(char *line)
{
	char	*path;
	int		i;

	i = 2;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	path = ft_strdup(&line[i]);
	if (path && path[ft_strlen(path) - 1] == '\n')
		path[ft_strlen(path) - 1] = '\0';
	return (path);
}

void	parse_identifiers(char *line, t_game *game)
{
	if (line[0] == '\n' || line[0] == '\0')
		return ;
	if (ft_strncmp(line, "NO ", 3) == 0 && !game->no_path)
		game->no_path = get_path(line);
	else if (ft_strncmp(line, "SO ", 3) == 0 && !game->so_path)
		game->so_path = get_path(line);
	else if (ft_strncmp(line, "WE ", 3) == 0 && !game->we_path)
		game->we_path = get_path(line);
	else if (ft_strncmp(line, "EA ", 3) == 0 && !game->ea_path)
		game->ea_path = get_path(line);
	else if (ft_strncmp(line, "F ", 2) == 0 && game->floor_color == -1)
		game->floor_color = parse_color(line, game);
	else if (ft_strncmp(line, "C ", 2) == 0 && game->ceiling_color == -1)
		game->ceiling_color = parse_color(line, game);
	else
		error_exit(game, "Invalid element or double element");
}

int	all_textures_parsed(t_game *game)
{
	return (game->no_path && game->so_path && game->we_path
		&& game->ea_path && game->floor_color != -1
		&& game->ceiling_color != -1);
}

static void	read_lines(t_game *game)
{
	char	*temp;

	game->current_line = get_next_line(game->fd);
	while (game->current_line)
	{
		if (!all_textures_parsed(game))
			parse_identifiers(game->current_line, game);
		else if (game->current_line[0] != '\n')
		{
			temp = ft_strjoin(game->map_buffer, game->current_line);
			free(game->map_buffer);
			game->map_buffer = temp;
		}
		free(game->current_line);
		game->current_line = get_next_line(game->fd);
	}
}

void	parse_cub_file(char *filename, t_game *game)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4 || ft_strncmp(&filename[len - 4], ".cub", 4) != 0)
		error_exit(game, "Invalid file extension.");
	game->fd = open(filename, O_RDONLY);
	if (game->fd < 0)
		error_exit(game, "Cannot open file.");
	game->map_buffer = ft_strdup("");
	read_lines(game);
	close(game->fd);
	game->fd = -1;
	if (!all_textures_parsed(game) || game->map_buffer[0] == '\0')
		error_exit(game, "Missing data, empty map or directory.");
	game->map = ft_split(game->map_buffer, '\n');
	free(game->map_buffer);
	game->map_buffer = NULL;
	if (!game->map || !game->map[0])
		error_exit(game, "Error splitting map");
	check_map_characters(game);
	init_player(game);
	game->y = 0;
	while (game->map[game->y])
		game->y++;
	check_map_closed(game);
}
