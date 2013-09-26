
#include "counter-clockwise.h"

int counter_clockwise (const struct point *a, const struct point *b, const struct point *c)
{
	double area;
	area = (b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x);
	if (area < 0) {
		/* clock wise */
		return -1;
	} else if (area > 0) {
		/* counter-clockwise */
		return 1;
	} else {
		/* collinear */
		return 0;
	}
}
