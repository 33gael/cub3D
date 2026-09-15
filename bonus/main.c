#include "bonus.h"

int main(int argc, char **argv)
{
	t_bonus *g;
	int ok;

	if (argc != 2)
		return (fprintf(stderr, "Usage: %s <map.cub>\n", argv[0]), 1);
	g = calloc(1, sizeof(*g));
	if (!g)
		return (1);
	g->minimap = 1;
	g->focused = 1;
	ok = load_map(g, argv[1]);
	if (ok)
	{
		ok = start_window(g);
	}
	destroy_window(g);
	free(g);
	return (!ok);
}
