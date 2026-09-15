#include "bonus.h"

void pixel(t_bonus *g, int x, int y, uint32_t color)
{
	if (x >= 0 && y >= 0 && x < BW && y < BH)
		g->pixels[y * BW + x] = color;
}

void rect(t_bonus *g, int x, int y, int w, int h, uint32_t color)
{
	for (int py = (int)fmax(0, y); py < (int)fmin(BH, y + h); ++py)
		for (int px = (int)fmax(0, x); px < (int)fmin(BW, x + w); ++px)
			pixel(g, px, py, color);
}

static void planes(t_bonus *g)
{
	rect(g, 0, 0, BW, VIEW / 2, g->ceiling_color);
	rect(g, 0, VIEW / 2, BW, VIEW / 2, g->floor_color);
}

static void walls(t_bonus *g)
{
	t_hit h;
	double camera, height, top, light, step, pos;
	double dx = cos(g->angle), dy = sin(g->angle);
	t_material *tex;
	int tx, ty, end;
	for (int x = 0; x < BW; ++x)
	{
		camera = (2.0 * (x + 0.5) / BW - 1) * tan(PI / 6);
		h = cast(g, g->x, g->y, dx - dy * camera, dy + dx * camera);
		h.distance = fmax(0.02, h.distance);
		height = BW / (2 * tan(PI / 6)) / h.distance;
		top = (VIEW - height) / 2;
		end = (int)fmin(VIEW, top + height);
		tex = &g->textures[h.material];
		tx = (int)fmin(tex->width - 1, h.u * tex->width);
		step = tex->height / height;
		pos = ((int)fmax(0, top) + 0.5 - top) * step;
		light = (h.side ? 0.76 : 0.98) / (1 + h.distance * 0.10);
		for (int y = (int)fmax(0, top); y < end; ++y)
		{
			ty = (int)fmin(tex->height - 1, fmax(0, pos));
			pos += step;
			pixel(g, x, y, shade(tex->pixels[ty * tex->width + tx], light));
		}
	}
}

void render(t_bonus *g)
{
	planes(g);
	walls(g);
	render_hud(g);
}
