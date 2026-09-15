#include "bonus.h"
#include <ctype.h>
#include <errno.h>

char cell(t_bonus *g, int x, int y)
{
	if (x < 0 || y < 0 || x >= g->width || y >= g->height)
		return (' ');
	return (g->map[y][x]);
}

static int map_error(const char *s)
{
	fprintf(stderr, "Error\nBonus: %s\n", s);
	return (0);
}

static int color_value(char *s, int *color)
{
	long v;
	int i;
	char *end;
	*color = 0;
	for (i = 0; i < 3; ++i)
	{
		while (isspace((unsigned char)*s))
			++s;
		if (!isdigit((unsigned char)*s))
			return (0);
		errno = 0;
		v = strtol(s, &end, 10);
		if (errno || v > 255)
			return (0);
		*color = (*color << 8) | (int)v;
		s = end;
		while (isspace((unsigned char)*s))
			++s;
		if (i < 2 && *s++ != ',')
			return (0);
	}
	return (*s == 0);
}

static int header(t_bonus *g, char *s, int *mask)
{
	const char *ids[] = {"NO", "SO", "WE", "EA", "F", "C"};
	size_t n;
	int i;
	for (i = 0; i < 6; ++i)
	{
		n = strlen(ids[i]);
		if (strncmp(s, ids[i], n) || !isspace((unsigned char)s[n]))
			continue ;
		if (*mask & (1 << i))
			return (0);
		*mask |= 1 << i;
		s += n;
		while (isspace((unsigned char)*s))
			++s;
		if (i >= 4)
			return (color_value(s, i == 4 ? &g->floor_color : &g->ceiling_color));
		if (!*s || strlen(s) >= sizeof(g->paths[i]))
			return (0);
		strcpy(g->paths[i], s);
		return (1);
	}
	return (0);
}

static int entities(t_bonus *g)
{
	int x, y, players = 0;
	char c;
	t_door *d;
	for (y = 0; y < g->height; ++y)
		for (x = 0; x < g->width; ++x)
		{
			c = cell(g, x, y);
			if (!strchr(" 01NSEWD", c))
				return (map_error("caractere de carte invalide"));
			if (strchr("NSEW", c))
			{
				++players;
				g->spawn_x = x + 0.5;
				g->spawn_y = y + 0.5;
				g->spawn_angle = (c == 'S') * PI / 2 + (c == 'W') * PI
					+ (c == 'N') * PI * 1.5;
				g->map[y][x] = '0';
			}
			if (c == 'D')
			{
				if (g->door_count == DOORS)
					return (map_error("trop de portes (128 maximum)"));
				d = &g->doors[g->door_count++];
				d->x = x;
				d->y = y;
				d->axis = cell(g, x, y - 1) == '1' && cell(g, x, y + 1) == '1';
				if (!d->axis && !(cell(g, x - 1, y) == '1' && cell(g, x + 1, y) == '1'))
					return (map_error("une porte doit etre encadree par deux murs opposes"));
			}
			if (c != ' ' && c != '1')
				for (int oy = -1; oy <= 1; ++oy)
					for (int ox = -1; ox <= 1; ++ox)
						if (cell(g, x + ox, y + oy) == ' ')
							return (map_error("carte ouverte ou sol adjacent au vide"));
		}
	if (players != 1)
		return (map_error("exactement un depart N/S/E/W requis"));
	return (1);
}

int load_map(t_bonus *g, const char *path)
{
	FILE *file;
	char *line = NULL, *s;
	size_t cap = 0, len;
	int mask = 0, started = 0, ended = 0, ok = 1;
	if (strlen(path) < 4 || strcmp(path + strlen(path) - 4, ".cub"))
		return (map_error("extension .cub requise"));
	file = fopen(path, "r");
	if (!file)
		return (map_error("impossible d'ouvrir la carte"));
	memset(g->map, ' ', sizeof(g->map));
	while (getline(&line, &cap, file) >= 0 && ok)
	{
		len = strlen(line);
		while (len && isspace((unsigned char)line[len - 1]))
			line[--len] = 0;
		s = line;
		while (*s == ' ' || *s == '\t')
			++s;
		if (!*s)
		{
			ended |= started;
			continue ;
		}
		if (mask != 63)
			ok = header(g, s, &mask);
		else if (ended || len >= MAP_MAX || g->height >= MAP_MAX)
			ok = 0;
		else
		{
			started = 1;
			memcpy(g->map[g->height++], line, len);
			if ((int)len > g->width)
				g->width = len;
		}
	}
	ok = ok && !ferror(file) && mask == 63 && started;
	free(line);
	fclose(file);
	if (!ok)
		return (map_error("entete invalide, carte vide, trop grande ou lignes separees"));
	if (!entities(g))
		return (0);
	reset_game(g);
	return (1);
}

void reset_game(t_bonus *g)
{
	g->x = g->spawn_x;
	g->y = g->spawn_y;
	g->angle = g->spawn_angle;
	memset(g->keys, 0, sizeof(g->keys));
	for (int i = 0; i < g->door_count; ++i)
	{
		g->doors[i].open = 0;
		g->doors[i].target = 0;
	}
}
