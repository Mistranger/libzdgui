#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "dimension.h"
#include "graphics.h"
#include "event.h"
#include "util/list.h"

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

/*
 * * Widget flags (may be used by user)
*/
         
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


/**
 * @class guiWidget_s
 * @date 21/01/2017
 * @file widget.h
 * @brief Generic widget structure. Used as a base class by other widget types.
 */
typedef struct guiWidget_s
{
	// List holding all listeners for widget.
	list_t *eventListeners;
	// Dimensions of widget (position and size)
    guiRectangle_t dim;
	// Parent widget (e.g. container)
	struct guiWidget_s *parent;

    // Flags
    int iflags;
    int flags;
	// Virtual functions table
	guiWidget_vf_t *v;
} guiWidget_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

#define widget_addMouseListener(widget, eventType, mouseType, listenerFunc) \
{\
	eventListener_t *listener = (eventListener_t*)malloc(sizeof(eventListener_t));\
	listener->type = eventType;\
	mouseListener_t *mouseListener = (mouseListener_t*)malloc(sizeof(mouseListener_t));\
	mouseListener->type = mouseType;\
	mouseListener->types.mousePressed = listenerFunc;\
	listener->listeners.mouseListener = mouseListener;\
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void*)listener);\
}

/**
 * @brief              Constructor (widget initialization)
 * @param widget       widget to be init
 */
void widget_init(guiWidget_t *widget);

// Virtual functions
guiRectangle_t widget_getChildrenArea(const guiWidget_t *widget);
guiWidget_t* widget_getWidgetAt(const guiWidget_t *widget, int x, int y);
void widget_draw(const guiWidget_t *widget, guiGraphics_t *graphics);
void widget_tick(guiWidget_t *widget);
bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist);

// Dimension management

/**
 * @brief              Sets widget width and creates a dimension event
 * @param widget       widget to change
 * @param width        new width
 */
void widget_setWidth(guiWidget_t *widget, int width);
/**
 * @brief              Sets widget height and creates a dimension event
 * @param widget       widget to change
 * @param height        new height
 */
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

// Children widgets management

// Parent widget access
guiWidget_t* widget_getParent(guiWidget_t *widget);
void widget_setParent(guiWidget_t *widget, guiWidget_t* newParent);

list_t* widget_getListeners(guiWidget_t *widget);
void widget_handleEvent(guiWidget_t *widget, event_t *event);

#endif // WIDGET_H_INCLUDED
