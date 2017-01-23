#include "system.h"

#include "dimension.h"

bool rect_isPointInRect(const guiRectangle_t *rect, vec2i_t pos)
{
	return ((pos.x >= rect->pos.x) && (pos.y >= rect->pos.y)
		&& pos.x < (rect->pos.x + rect->width)
		&& pos.y < (rect->pos.y + rect->height));
}

bool rect_intersect(guiRectangle_t* rect, guiRectangle_t* other)
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
	
	if (rect->pos.x + rect->width > other->width) {
		rect->width = other->width - rect->pos.x;
        }

	if (rect->pos.y + rect->height > other->height) {
		rect->height = other->height - rect->pos.y;
	}
	
	if (rect->width <= 0 || rect->height <= 0) {
		rect->height = 0;
		rect->width = 0;
		rect->pos.x += other->pos.x;
		rect->pos.y += other->pos.y;
		return false;
	}

	rect->pos.x += other->pos.x;
	rect->pos.y += other->pos.y;
	return true;
}
