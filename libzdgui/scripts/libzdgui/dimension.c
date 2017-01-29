#include "system.h"

#include "dimension.h"
#include "util/util.h"

void rect_intersect(guiRectangle *rect, const guiRectangle *other)
{
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
	
	rect->pos.x += other->pos.x;
	rect->pos.y += other->pos.y;
}
