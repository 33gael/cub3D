/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeducas <gaeducas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 17:46:44 by thbosvie          #+#    #+#             */
/*   Updated: 2026/07/17 16:34:56 by gaeducas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# define _POSIX_C_SOURCE 200809L

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define DEBUG 0

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define PI 3.14159265359

# include "libft.h"
# include "mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <time.h>

typedef struct s_player
{
	float		x;
	float		y;
	float		angle;
	bool		key_up;
	bool		key_down;
	bool		key_left;
	bool		key_right;
	bool		left_rotate;
	bool		right_rotate;
}				t_player;

typedef struct s_texture
{
	void		*img_ptr;
	char		*data;
	int			width;
	int			height;
	int			bpp;
	int			line_len;
	int			endian;
}				t_texture;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	t_player	player;
	t_texture	tex_n;
	t_texture	tex_s;
	t_texture	tex_e;
	t_texture	tex_w;
	char		**map;
	int			y;
	char		*no_path;
	char		*so_path;
	char		*we_path;
	char		*ea_path;
	int			floor_color;
	int			ceiling_color;
	char		*map_buffer;
	char		*current_line;
	int			fd;
	double		last_frame;
	double		delta_time;
	bool		focused;
}				t_game;

typedef struct s_ray
{
	float		ray_x;
	float		ray_y;
	int			map_x;
	int			map_y;
	float		side_dx;
	float		side_dy;
	float		delta_dx;
	float		delta_dy;
	int			step_x;
	int			step_y;
	int			side;
	float		dist;
	float		wall_x;
	float		height;
	int			start_y;
	int			end;
	int			tex_x;
	t_texture	*tex;
}				t_ray;

typedef struct s_point
{
	int			x;
	int			y;
}				t_point;

int				close_game(t_game *game);
void			put_pixel(int x, int y, int color, t_game *game);
void			clear_image(t_game *game);
void			draw_square(t_point p, int size, int color, t_game *game);
void			draw_map(t_game *game);
void			draw_3d_dda(int x, int start_y, int end_y, t_game *mapa);
void			load_texture(t_game *game, t_texture *tex, char *path);
void			load_all_textures(t_game *game);
void			init_game(t_game *game, char *map_file);
int				get_texture_pixel(t_texture *tex, int x, int y);
int				sample_texture(t_texture *tex, float x, float y);
void			draw_line(t_player *p, t_game *g, float start_x, int i);
void			move_player(t_game *game);
void			init_timing(t_game *game);
void			frame_time(t_game *game);
int				key_press(int keycode, t_game *game);
int				key_release(int keycode, t_game *game);

bool			touch(float px, float py, t_game *game);

void			init_player(t_game *game);
void			parse_cub_file(char *filename, t_game *game);
int				parse_color(char *line, t_game *game);
void			check_map_closed(t_game *game);
void			check_map_characters(t_game *game);

void			error_exit(t_game *game, char *message);
void			free_map(char **map);
char			**duplicate_map(char **map, int height);
void			free_game(t_game *game);

#endif
