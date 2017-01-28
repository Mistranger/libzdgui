#include "system.h"

#include "dimension.h"
#include "util/util.h"

void rect_intersect(guiRectangle *rect, const guiRectangle *other)
{
	/*
	int x5 = MAX(rect->pos.x, other->pos.x);
	int y5 = MAX(rect->pos.y, other->pos.y);
	int x6 = MIN(rect->pos.x + rect->width, other->pos.x + other->width);
	int y6 = MIN(rect->pos.y + rect->height, other->pos.y + other->height);
	rect->pos.x = x5 >= x6 ? x6 : x5;
	rect->width = x5 >= x6 ? x5 - x6 :  x6 - x5;
	rect->pos.y = y5 >= y6 ? y6 : y5;
	rect->height = y5 >= y6 ? y5 - y6 :  y6 - y5;
	*/

	rect->pos.x -= other->pos.x;
	rect->pos.y -= other->pos.y;

	if (rect->pos.x < 0) {
		rect->width += rect->pos.x;
		rect->pos.x = 0;
	}

	if (rect->pos.y < 0) {
		rect->height += rect->pos.y;
		rect->pos.y = 0;
	}

	rect->width = (rect->pos.x + rect->width > other->width) ? (other->width - rect->pos.x) : rect->width;
	rect->height = (rect->pos.y + rect->height > other->height) ? (other->height - rect->pos.y) : rect->height;


	/*if (rect->width <= 0 || rect->height <= 0) {
		rect->height = 0;
		rect->width = 0;
		rect->pos.x += other->pos.x;
		rect->pos.y += other->pos.y;
		return false;
	}*/

	rect->pos.x += other->pos.x;
	rect->pos.y += other->pos.y;
}
