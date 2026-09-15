#ifndef BONUS_H
# define BONUS_H

# define _POSIX_C_SOURCE 200809L
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <stdint.h>
# define BW 1280
# define BH 720
# define VIEW BH
# define MAP_MAX 128
# define DOORS 128
# define PI 3.14159265358979323846

typedef struct s_door
{
	int x, y, axis, target;
	double open;
} t_door;

typedef struct s_hit
{
	double distance, u;
	int material, side;
} t_hit;

typedef struct s_material
{
	uint32_t *pixels;
	int width, height;
} t_material;

typedef struct s_bonus
{
	void *mlx, *win, *image;
	char *image_data;
	int bpp, stride, endian;
	char map[MAP_MAX][MAP_MAX];
	int width, height, door_count;
	int keys[65536], focused, minimap, show_fps, running;
	int floor_color, ceiling_color;
	char paths[4][512];
	t_door doors[DOORS];
	double x, y, angle, spawn_x, spawn_y, spawn_angle;
	double last_time, fps_time, fps;
	int frames;
	uint32_t pixels[BW * BH];
	t_material textures[5];
} t_bonus;

int load_map(t_bonus *g, const char *path);
void reset_game(t_bonus *g);
char cell(t_bonus *g, int x, int y);
t_door *door_at(t_bonus *g, int x, int y);
t_hit cast(t_bonus *g, double x, double y, double dx, double dy);
int walkable(t_bonus *g, double x, double y, double radius);
void move_actor(t_bonus *g, double *x, double *y, double dx, double dy);
void update_game(t_bonus *g, double dt);
void interact(t_bonus *g);
void render(t_bonus *g);
void render_hud(t_bonus *g);
void pixel(t_bonus *g, int x, int y, uint32_t color);
void rect(t_bonus *g, int x, int y, int w, int h, uint32_t color);
void label(t_bonus *g, int x, int y, const char *s, int scale, int color);
uint32_t shade(uint32_t c, double light);
int start_window(t_bonus *g);
void destroy_window(t_bonus *g);

#endif
