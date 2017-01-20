#include "libzdgui.h"
#include "dimension.h"

bool rect_isPointInRect(const guiRectangle_t *rect, int x, int y)
{
	return ((x >= rect->x) && (y >= rect->y)
		&& x < (rect->x + rect->width)
		&& y < (rect->y + rect->height));
}
