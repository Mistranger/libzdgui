#include "system.h"
#include "dimension.h"

bool rect_isPointInRect(const guiRectangle_t *rect, int x, int y)
{
	return ((x >= rect->x) && (y >= rect->y)
		&& x < (rect->x + rect->width)
		&& y < (rect->y + rect->height));
}

bool rect_intersect(guiRectangle_t* rect, guiRectangle_t* other)
{
	rect->x -= other->x;
	rect->y -= other->y;
	
	if (rect->x < 0) {
		rect->width += rect->x;
		rect->x = 0;
	}

	if (rect->y < 0) {
		rect->height += rect->y;
		rect->y = 0;
	}
	
	if (rect->x + rect->width > other->width) {
		rect->width = other->width - rect->x;
        }

	if (rect->y + rect->height > other->height) {
		rect->height = other->height - rect->y;
	}
	
	if (rect->width <= 0 || rect->height <= 0) {
		rect->height = 0;
		rect->width = 0;
		rect->x += other->x;
		rect->y += other->y;
		return false;
	}

	rect->x += other->x;
	rect->y += other->y;
	return true;
}
