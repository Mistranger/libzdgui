#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include "dimension.h"
#include "graphics.h"
#include "event.h"
#include "util/list.h"

enum WidgetFlags {
	WF_VISIBLE = 0x00000001,
	WF_ENABLED = 0x00000002,
	WF_FOCUSABLE = 0x00000004
};

typedef struct guiWidget_vf {
	
	struct guiRectangle_s (*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, int x, int y);
	void (*w_draw)(const struct guiWidget_s *widget, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
} guiWidget_vf_t;


typedef struct guiWidget_s
{
	list_t *eventListeners;
    guiRectangle_t dim;
	struct guiWidget_s *parent;

    // Flags
    int iflags;
    int flags;
	// Virtual functions
	guiWidget_vf_t *v;
} guiWidget_t;

// Functions

void widget_init(guiWidget_t *widget);

// Position management

void widget_setWidth(guiWidget_t *widget, int width);
void widget_setHeight(guiWidget_t *widget, int height);
int widget_getWidth(guiWidget_t *widget);
int widget_getHeight(guiWidget_t *widget);
void widget_setX(guiWidget_t *widget, int x);
void widget_setY(guiWidget_t *widget, int y);
int widget_getX(guiWidget_t *widget);
int widget_getY(guiWidget_t *widget);
void widget_setPosition(guiWidget_t *widget, int x, int y);
void widget_setSize(guiWidget_t *widget, int width, int height);
void widget_setDimension(guiWidget_t *widget, const guiRectangle_t newDim);
guiRectangle_t widget_getDimensions(const guiWidget_t *widget);
void widget_getAbsolutePosition(guiWidget_t* widget, int *x, int *y);
guiRectangle_t widget_getChildrenArea(const guiWidget_t *widget);

guiWidget_t* widget_getWidgetAt(const guiWidget_t *widget, int x, int y);
bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist);

// Parent widget access
guiWidget_t* widget_getParent(guiWidget_t *widget);
void widget_setParent(guiWidget_t *widget, guiWidget_t* newParent);

void widget_tick(guiWidget_t *widget);
void widget_draw(const guiWidget_t *widget, guiGraphics_t *graphics);

list_t* widget_getListeners(guiWidget_t *widget);
void widget_handleEvent(guiWidget_t *widget, event_t *event);



#endif // WIDGET_H_INCLUDED
