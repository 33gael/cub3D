#include "bonus.h"
#include "mlx.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

static double now(void)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec + t.tv_nsec / 1e9);
}

static int quit(t_bonus *g)
{
	g->running = 0;
	mlx_loop_end(g->mlx);
	return (0);
}

static int key_down(int key, t_bonus *g)
{
	if (key < 0 || key >= 65536)
		return (0);
	if (key >= 'A' && key <= 'Z')
		key += 'a' - 'A';
	if (key == 65307)
		return (quit(g));
	if (!g->keys[key])
	{
		if (key == 'e')
			interact(g);
		if (key == 'f')
			g->show_fps = !g->show_fps;
		if (key == 'm')
			g->minimap = !g->minimap;
		if (key == 'r')
			reset_game(g);
	}
	g->keys[key] = 1;
	return (0);
}

static int key_up(int key, t_bonus *g)
{
	if (key >= 'A' && key <= 'Z')
		key += 'a' - 'A';
	if (key >= 0 && key < 65536)
		g->keys[key] = 0;
	return (0);
}

static int focus_out(t_bonus *g)
{
	g->focused = 0;
	memset(g->keys, 0, sizeof(g->keys));
	return (0);
}

static int focus_in(t_bonus *g)
{
	g->focused = 1;
	g->last_time = now();
	return (0);
}

static void present(t_bonus *g)
{
	uint32_t c;
	unsigned char *p;
	int bytes = g->bpp / 8, shift;
	if (g->bpp == 32 && g->endian == 0)
	{
		for (int y = 0; y < BH; ++y)
			memcpy(g->image_data + y * g->stride, g->pixels + y * BW, BW * 4);
		mlx_put_image_to_window(g->mlx, g->win, g->image, 0, 0);
		return ;
	}
	for (int y = 0; y < BH; ++y)
		for (int x = 0; x < BW; ++x)
		{
			c = g->pixels[y * BW + x];
			p = (unsigned char *)g->image_data + y * g->stride + x * bytes;
			for (int b = 0; b < bytes; ++b)
			{
				shift = g->endian ? (bytes - b - 1) * 8 : b * 8;
				p[b] = c >> shift;
			}
		}
	mlx_put_image_to_window(g->mlx, g->win, g->image, 0, 0);
}

static int frame(t_bonus *g)
{
	double time = now(), dt = time - g->last_time;
	struct timespec delay;
	if (!g->running)
		return (0);
	if (dt < 1.0 / 60)
	{
		delay.tv_sec = 0;
		delay.tv_nsec = (long)((1.0 / 60 - dt) * 1e9);
		nanosleep(&delay, NULL);
		time = now();
		dt = time - g->last_time;
	}
	g->last_time = time;
	g->fps_time += dt;
	++g->frames;
	if (g->fps_time >= 0.5)
	{
		g->fps = g->frames / g->fps_time;
		g->fps_time = 0;
		g->frames = 0;
	}
	update_game(g, dt);
	render(g);
	present(g);
	return (0);
}

static int load_custom(t_bonus *g)
{
	void *image;
	char *data;
	int w, h, bpp, stride, endian;
	uint32_t color;
	char *path;
	for (int i = 0; i < 5; ++i)
	{
		path = i == 4 ? "./Textures/bonus_door.xpm" : g->paths[i];
		image = mlx_xpm_file_to_image(g->mlx, path, &w, &h);
		if (!image)
			return (fprintf(stderr, "Error\nTexture: %s\n", path), 0);
		data = mlx_get_data_addr(image, &bpp, &stride, &endian);
		if (!data || w <= 0 || h <= 0 || (bpp != 24 && bpp != 32))
			return (mlx_destroy_image(g->mlx, image), 0);
		g->textures[i].width = w;
		g->textures[i].height = h;
		g->textures[i].pixels = malloc((size_t)w * h * sizeof(uint32_t));
		if (!g->textures[i].pixels)
			return (mlx_destroy_image(g->mlx, image), 0);
		for (int y = 0; y < h; ++y)
			for (int x = 0; x < w; ++x)
			{
				color = 0;
				for (int b = 0; b < bpp / 8; ++b)
					color |= (uint32_t)(unsigned char)data[y * stride + x * (bpp / 8) + b]
						<< (endian ? (bpp / 8 - b - 1) * 8 : b * 8);
				g->textures[i].pixels[y * w + x] = color & 0xffffff;
			}
		mlx_destroy_image(g->mlx, image);
	}
	return (1);
}

int start_window(t_bonus *g)
{
	Bool supported;
	g->mlx = mlx_init();
	if (!g->mlx)
		return (fprintf(stderr, "Error\nAffichage X11 indisponible.\n"), 0);
	XkbSetDetectableAutoRepeat(*(Display **)g->mlx, True, &supported);
	if (!load_custom(g))
		return (0);
	g->win = mlx_new_window(g->mlx, BW, BH, "cub3D bonus");
	if (!g->win)
		return (0);
	g->image = mlx_new_image(g->mlx, BW, BH);
	if (!g->image)
		return (0);
	g->image_data = mlx_get_data_addr(g->image, &g->bpp, &g->stride, &g->endian);
	if (!g->image_data || (g->bpp != 24 && g->bpp != 32))
		return (0);
	g->running = 1;
	g->focused = 1;
	g->last_time = now();
	mlx_hook(g->win, 17, 0, quit, g);
	mlx_hook(g->win, 2, 1L << 0, key_down, g);
	mlx_hook(g->win, 3, 1L << 1, key_up, g);
	mlx_hook(g->win, 9, 1L << 21, focus_in, g);
	mlx_hook(g->win, 10, 1L << 21, focus_out, g);
	mlx_loop_hook(g->mlx, frame, g);
	mlx_loop(g->mlx);
	return (1);
}

void destroy_window(t_bonus *g)
{
	for (int i = 0; i < 5; ++i)
		free(g->textures[i].pixels);
	if (g->image)
		mlx_destroy_image(g->mlx, g->image);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
}
