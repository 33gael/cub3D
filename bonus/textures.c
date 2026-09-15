#include "bonus.h"

uint32_t shade(uint32_t c, double light)
{
	light = fmax(0, fmin(1, light));
	return ((int)(((c >> 16) & 255) * light) << 16
		| (int)(((c >> 8) & 255) * light) << 8
		| (int)((c & 255) * light));
}
