#include "bonus.h"

t_door *door_at(t_bonus *g, int x, int y)
{
	for (int i = 0; i < g->door_count; ++i)
		if (g->doors[i].x == x && g->doors[i].y == y)
			return (&g->doors[i]);
	return (NULL);
}

static int hit_door(t_door *d, double x, double y, double dx, double dy, t_hit *h)
{
	double t, u, direction;
	direction = d->axis ? dx : dy;
	if (fabs(direction) < 1e-9 || d->open >= 1)
		return (0);
	t = ((d->axis ? d->x : d->y) + 0.5 - (d->axis ? x : y)) / direction;
	u = d->axis ? y + t * dy - d->y : x + t * dx - d->x;
	if (t < 0 || u < d->open || u >= 1)
		return (0);
	h->distance = t;
	h->u = u - d->open;
	h->side = !d->axis;
	h->material = 4;
	return (1);
}

t_hit cast(t_bonus *g, double x, double y, double dx, double dy)
{
	int mx = (int)floor(x), my = (int)floor(y);
	int sx = dx < 0 ? -1 : 1, sy = dy < 0 ? -1 : 1;
	double ddx = fabs(dx) < 1e-9 ? 1e30 : fabs(1 / dx);
	double ddy = fabs(dy) < 1e-9 ? 1e30 : fabs(1 / dy);
	double tx = (dx < 0 ? x - mx : mx + 1 - x) * ddx;
	double ty = (dy < 0 ? y - my : my + 1 - y) * ddy;
	t_hit h = {0.001, 0, 0, 0};
	t_door *d;
	for (int i = 0; i < MAP_MAX * 2 + 2; ++i)
	{
		d = door_at(g, mx, my);
		if (d && hit_door(d, x, y, dx, dy, &h))
			return (h);
		if (cell(g, mx, my) == '1' || cell(g, mx, my) == ' ')
		{
			h.u = h.side ? x + h.distance * dx : y + h.distance * dy;
			h.u -= floor(h.u);
			h.material = h.side ? (dy > 0 ? 0 : 1) : (dx > 0 ? 2 : 3);
			return (h);
		}
		h.side = tx >= ty;
		if (tx < ty)
		{
			h.distance = tx;
			tx += ddx;
			mx += sx;
		}
		else
		{
			h.distance = ty;
			ty += ddy;
			my += sy;
		}
	}
	return (h);
}

int walkable(t_bonus *g, double x, double y, double radius)
{
	t_door *d;
	char c;
	double near_x, near_y;
	for (int ty = (int)floor(y - radius); ty <= (int)floor(y + radius); ++ty)
		for (int tx = (int)floor(x - radius); tx <= (int)floor(x + radius); ++tx)
		{
			c = cell(g, tx, ty);
			if (c == '1' || c == ' ')
				return (0);
			d = door_at(g, tx, ty);
			if (!d || d->open >= 1)
				continue ;
			near_x = d->axis ? tx + 0.5 : fmax(tx + d->open, fmin(x, tx + 1));
			near_y = d->axis ? fmax(ty + d->open, fmin(y, ty + 1)) : ty + 0.5;
			if (hypot(x - near_x, y - near_y) < radius + 0.04)
				return (0);
		}
	return (1);
}

void move_actor(t_bonus *g, double *x, double *y, double dx, double dy)
{
	if (walkable(g, *x + dx, *y, 0.20))
		*x += dx;
	if (walkable(g, *x, *y + dy, 0.20))
		*y += dy;
}

void interact(t_bonus *g)
{
	t_door *d;
	double dx, dy, distance, best = 1.65;
	int selected = -1;
	for (int i = 0; i < g->door_count; ++i)
	{
		d = &g->doors[i];
		dx = d->x + 0.5 - g->x;
		dy = d->y + 0.5 - g->y;
		distance = hypot(dx, dy);
		if (distance < best && (distance < 0.4
				|| (dx * cos(g->angle) + dy * sin(g->angle)) / distance > 0.45)
			&& cast(g, g->x, g->y, dx, dy).distance >= 0.95)
		{
			selected = i;
			best = distance;
		}
	}
	if (selected >= 0)
		g->doors[selected].target = !g->doors[selected].target;
}

static void update_doors(t_bonus *g, double dt)
{
	t_door *d;
	int occupied;
	for (int i = 0; i < g->door_count; ++i)
	{
		d = &g->doors[i];
		occupied = fabs(g->x - d->x - 0.5) < 0.8 && fabs(g->y - d->y - 0.5) < 0.8;
		if (d->target)
			d->open = fmin(1, d->open + dt * 1.25);
		else if (!occupied)
			d->open = fmax(0, d->open - dt * 1.25);
	}
}

void update_game(t_bonus *g, double dt)
{
	double forward, side, length, speed;
	if (!g->focused)
		return ;
	dt = fmin(dt, 0.05);
	g->angle += (g->keys[65363] - g->keys[65361]) * dt * 2.2;
	g->angle = remainder(g->angle, 2 * PI);
	forward = g->keys['w'] - g->keys['s'];
	side = g->keys['d'] - g->keys['a'];
	length = hypot(forward, side);
	speed = 2.5;
	if (length > 0)
		move_actor(g, &g->x, &g->y,
			(cos(g->angle) * forward - sin(g->angle) * side) * dt * speed / length,
			(sin(g->angle) * forward + cos(g->angle) * side) * dt * speed / length);
	update_doors(g, dt);
}
