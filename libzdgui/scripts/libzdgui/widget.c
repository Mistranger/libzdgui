#include "libzdgui.h"

#include "widget.h"

void widget_setWidth(guiWidget_t* widget, int width)
{
}

void widget_setHeight(guiWidget_t* widget, int height)
{
}

int widget_getWidth(guiWidget_t* widget)
{
	return widget->dim.width;
}

int widget_getHeight(guiWidget_t* widget)
{
	return widget->dim.height;
}

void widget_setX(guiWidget_t* widget, int x)
{
}

void widget_setY(guiWidget_t* widget, int y)
{
}

int widget_getX(guiWidget_t* widget)
{
	return widget->dim.x;
}

int widget_getY(guiWidget_t* widget)
{
	return widget->dim.y;
}

guiRectangle_t widget_getDimensions(const guiWidget_t* widget)
{
	guiRectangle_t rect = widget->dim;
	return rect;
}


void widget_setParent(guiWidget_t* widget, guiWidget_t* newParent)
{
	widget->parent = newParent;
}

guiWidget_t* widget_getParent(guiWidget_t* widget)
{
	return widget->parent;
}

void widget_getAbsolutePosition(guiWidget_t* widget, int *x, int *y)
{
	if (!widget->parent){ 
		*x = widget->dim.x;
		*y = widget->dim.y;
		return;
	}

	int parentX;
	int parentY;

	widget_getAbsolutePosition(widget->parent, &parentX, &parentY);

	*x = parentX + widget->dim.x + (widget->parent->guiWidget_getChildrenArea)(widget->parent)->x;
	*y = parentY + widget->dim.y + (widget->parent->guiWidget_getChildrenArea)(widget->parent)->y;
}
