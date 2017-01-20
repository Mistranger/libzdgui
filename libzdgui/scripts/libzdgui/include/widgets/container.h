#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED

#include "util/list.h"
#include "util/util.h"
#include "widget.h"

typedef struct guiContainer_s
{
    guiWidget_t *widget;
	
	
	// All contained widgets
	list_t *children;
	
} guiContainer_t;

// Virtual inherited from guiWidget_t
void container_draw(const guiWidget_t *widget, guiGraphics_t *graphics);
guiRectangle_t* container_getChildrenArea(const guiWidget_t *widget);
guiWidget_t* container_getWidgetAt(const guiWidget_t *widget, int x, int y);
void container_tick(guiWidget_t *widget);

void container_init(guiContainer_t *container);
void container_add(guiContainer_t *container, guiWidget_t *widget);
void container_remove(guiContainer_t *container, guiWidget_t *widget);



#endif // CONTAINER_H_INCLUDED
