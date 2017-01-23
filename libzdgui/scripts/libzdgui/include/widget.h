#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "dimension.h"
#include "graphics.h"
#include "event.h"
#include "fonts.h"
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
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
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
	// Font used for drawing
	guiFont_t *font;
	// Virtual functions table
	guiWidget_vf_t *v;
} guiWidget_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

#define widget_addMouseListener(_widget, _eventType, _mouseType, _listenerFunc) \
{\
	eventListener_t *listener = new(eventListener_t);\
	listener->type = _eventType;\
	mouseListener_t *mouseListener = new(mouseListener_t);\
	mouseListener->type = _mouseType;\
	mouseListener->types.mousePressed = _listenerFunc;\
	listener->listeners.mouseListener = mouseListener;\
	list_push_back(((guiWidget_t *)_widget)->eventListeners, (void*)listener);\
}

#define widget_addDimensionListener(_widget, _eventType, _dimType, _listenerFunc) \
{\
	eventListener_t *listener = new(eventListener_t);\
	listener->type = _eventType;\
	dimensionListener_t *dimlistener = new(dimensionListener_t);\
	dimlistener->type = _dimType;\
	dimlistener->types.resized = _listenerFunc;\
	listener->listeners.dimensionListener = dimlistener;\
	list_push_back(((guiWidget_t *)_widget)->eventListeners, (void*)listener);\
}

/**
 * @brief              Constructor (widget initialization)
 * @param widget       widget to be init
 */
void widget_init(guiWidget_t *widget);

// Virtual functions
guiRectangle_t widget_getChildrenArea(const guiWidget_t *widget);
guiWidget_t* widget_getWidgetAt(const guiWidget_t *widget, vec2i_t pos);
void widget_draw(const guiWidget_t *widget, guiGraphics_t *graphics);
void widget_tick(guiWidget_t *widget);
bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist);

// Dimension management

void widget_setDimension(guiWidget_t *widget, const guiRectangle_t newDim);
void widget_getAbsolutePosition(guiWidget_t* widget, vec2i_t *x);

#define widget_setWidth(_widget, _width)         \
{ \
	guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim; \
	newDimension.width = _width;                \
	widget_setDimension(((guiWidget_t*)_widget), newDimension); \
}

#define widget_setHeight(_widget, height)       \
{ \
	guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim; \
	newDimension.height = height;              \
	widget_setDimension(((guiWidget_t*)_widget), newDimension); \
}

#define widget_setX(_widget, _x)         \
{ \
	guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim;  \
	newDimension.pos.x = _x;                     \
	widget_setDimension(((guiWidget_t*)_widget), newDimension);  \
}

#define widget_setY(_widget, _y)       \
{ \
	guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim;  \
	newDimension.pos.y = _y;                     \
	widget_setDimension(((guiWidget_t*)_widget), newDimension);  \
}

#define widget_setPosition(_widget, _pos)       \
{ \
	guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim;  \
	newDimension.pos = _pos;                     \
	widget_setDimension(((guiWidget_t*)_widget), newDimension);  \
}

#define widget_setSize(_widget, _width, _height)   \
{ \
	guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim;  \
	newDimension.width = _width;                 \
	newDimension.height = _height;               \
	widget_setDimension(((guiWidget_t*)_widget), newDimension);  \
}

#define widget_getWidth(_widget) (((guiWidget_t*)_widget)->dim.width)
#define widget_getHeight(_widget) (((guiWidget_t*)_widget)->dim.height)
#define widget_getX(_widget) (((guiWidget_t*)_widget)->dim.pos.x)
#define widget_getY(_widget) (((guiWidget_t*)_widget)->dim.pos.y)
#define widget_getDimensions(_widget) (((guiWidget_t*)_widget)->dim)

// Children widgets management

// Parent widget access
#define widget_getParent(_widget) (((guiWidget_t*)_widget)->parent)
#define widget_setParent(_widget, _newParent) { (guiWidget_t*)_widget->parent = (guiWidget_t*)_newParent; }

// Property access
#define widget_getFont(_widget) (((guiWidget_t*)_widget)->font)
#define widget_setFont(_widget, _font) { ((guiWidget_t*)_widget)->font = _font; }
#define widget_getListeners(_widget) (((guiWidget_t*)_widget)->eventListeners)

void widget_handleEvent(guiWidget_t *widget, event_t *event);

#endif // WIDGET_H_INCLUDED
