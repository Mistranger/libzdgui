#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include "dimension.h"
#include "graphics.h"
#include "util/list.h"

enum WidgetFlags {
	WF_VISIBLE = 0x00000001,
	WF_ENABLED = 0x00000002,
	WF_FOCUSABLE = 0x00000004
};

typedef struct guiWidget_s
{
	// Virtual functions
	void (*guiWidget_draw)(const struct guiWidget_s *widget, guiGraphics_t *graphics);
	guiRectangle_t* (*guiWidget_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*guiWidget_getWidgetAt)(const struct guiWidget_s *widget, int x, int y);
	void (*guiWidget_tick)(struct guiWidget_s *widget);
	
	list_t *eventListeners;
    guiRectangle_t dim;
	struct guiWidget_s *parent;

    // Flags
    int iflags;
    int flags;

} guiWidget_t;

// Functions

// Position management

void widget_setWidth(guiWidget_t *widget, int width);
void widget_setHeight(guiWidget_t *widget, int height);
int widget_getWidth(guiWidget_t *widget);
int widget_getHeight(guiWidget_t *widget);
void widget_setX(guiWidget_t *widget, int x);
void widget_setY(guiWidget_t *widget, int y);
int widget_getX(guiWidget_t *widget);
int widget_getY(guiWidget_t *widget);
guiRectangle_t widget_getDimensions(const guiWidget_t *widget);
void widget_getAbsolutePosition(guiWidget_t* widget, int *x, int *y);


// Parent widget access
guiWidget_t* widget_getParent(guiWidget_t *widget);
void widget_setParent(guiWidget_t *widget, guiWidget_t* newParent);


void guiWidget_draw(guiWidget_t *widget);

#endif // WIDGET_H_INCLUDED
