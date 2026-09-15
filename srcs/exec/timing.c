#include "cub3d.h"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

static double	current_time(void)
{
	struct timespec	t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec + t.tv_nsec / 1e9);
}

void	frame_time(t_game *g)
{
	struct timespec	delay;
	double			now;
	double			elapsed;

	now = current_time();
	elapsed = now - g->last_frame;
	if (elapsed < 1.0 / 60.0)
	{
		delay.tv_sec = 0;
		delay.tv_nsec = (long)((1.0 / 60.0 - elapsed) * 1e9);
		nanosleep(&delay, NULL);
		now = current_time();
	}
	g->delta_time = fmin(now - g->last_frame, 0.05);
	g->last_frame = now;
}

static int	focus_out(t_game *g)
{
	g->focused = false;
	g->player.key_up = false;
	g->player.key_down = false;
	g->player.key_left = false;
	g->player.key_right = false;
	g->player.left_rotate = false;
	g->player.right_rotate = false;
	return (0);
}

static int	focus_in(t_game *g)
{
	g->focused = true;
	g->last_frame = current_time();
	return (0);
}

void	init_timing(t_game *g)
{
	Bool	supported;

	g->focused = true;
	g->last_frame = current_time();
	XkbSetDetectableAutoRepeat(*(Display **)g->mlx, True, &supported);
	mlx_hook(g->win, 9, 1L << 21, focus_in, g);
	mlx_hook(g->win, 10, 1L << 21, focus_out, g);
}
