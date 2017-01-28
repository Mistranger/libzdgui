#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "dimension.h"
#include "graphics.h"
#include "event.h"
#include "event/event_dimension.h"
#include "event/event_lifecycle.h"
#include "event/event_mouse.h"
#include "fonts.h"
#include "util/list.h"

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef struct guiWidget_vf {
	const char *(*w_typename)(struct guiWidget_s *widget);
	void (*w_destructor)(struct guiWidget_s *widget);
	struct guiRectangle_s *(*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s *(*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void (*w_draw)(const struct guiWidget_s *widget, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
	void (*w_setFocusManager)(struct guiWidget_s *widget, void *focus);
} guiWidget_vf_t;


/**
 * @class guiWidget_s
 * @date 21/01/2017
 * @file widget.h
 * @brief Generic widget structure. Used as a base class by other widget types.
 */
typedef struct guiWidget_s {
	void *gui;                            /// GUI object (used for some system requests, e.g. cursor change)
	
	list_t *eventListeners;               /// List holding all listeners for widget.
	
	guiRectangle_t dim;                   /// Dimensions of widget (position and size)
	
	struct guiWidget_s *parent;           /// Parent widget (e.g. container)
	void *focusManager;                   /// Focus manager, which handles all focus events

	// Flags
	bool isContainer;                     /// Can contain other widgets
	bool isEnabled;                       /// Can handle events and interact with other widgets
	bool isFocusable;                     /// Handle focus gain/lose events (otherwise it won't handle other events)
	bool isVisible;                       /// Is visible on screen
	
	guiFont_t *font;                      /// Font used for drawing
	
	guiWidget_vf_t *v;                    /// Virtual functions table
} guiWidget_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/



/**
 * @brief              Constructor (widget initialization)
 * @param widget       widget to be init
 */
void widget_init(guiWidget_t *widget);



/**
 * @brief Widget destructor
 *
 * Handles the correct destruction of the widget.
 * However, this method is for internal use only, instead, use guiGui_t object
 * to delete a widget.
 *
 * @param widget  Widget to delete
 */
void widget_destructor(guiWidget_t *widget);
const char *widget_typename(guiWidget_t *widget);

/**
 * @brief Adds an event listener to widget
 *
 * Put full description here
 *
 * @param widget Put argument desc here
 * @param listener Put argument desc here
 * @return Put return information here
 */
void widget_addListener(void *widget, void *listener);
mouseListener_t *widget_addMouseListener(void *widget,
										 mouseEventType_t mouseType, void(*listenerFunc)(void *widget, mouseEvent_t *event));
dimensionListener_t *widget_addDimensionListener(void *widget,
												 dimensionEventType_t dimType, void(*listenerFunc)(void *widget, dimensionEvent_t *event));
lifecycleListener_t *widget_addLifeCycleListener(void *widget,
												 lifecycleEventType_t dimType, void(*listenerFunc)(void *widget, lifecycleEvent_t *event));
void widget_removeListener(void *widget, void *listener);

// Virtual functions
guiRectangle_t *widget_getChildrenArea(const guiWidget_t *widget);
guiWidget_t *widget_getWidgetAt(const guiWidget_t *widget, vec2i_t pos);
void widget_draw(const guiWidget_t *widget, guiGraphics_t *graphics);
void widget_tick(guiWidget_t *widget);
bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist);
#define widget_getFocusManager(_widget) (((guiWidget_t*)_widget)->focusManager)
void widget_setFocusManager(guiWidget_t *widget, void *focus);

void widget_requestFocus(guiWidget_t *widget);
void widget_requestModalFocus(guiWidget_t *widget);
void widget_requestModalMouseInputFocus(guiWidget_t *widget);
void widget_releaseModalFocus(guiWidget_t *widget);
void widget_releaseModalMouseInputFocus(guiWidget_t *widget);
bool widget_hasModalFocus(const guiWidget_t *widget);
bool widget_hasModalMouseInputFocus(const guiWidget_t *widget);
// Flags

#define widget_isEnabled(_widget) (((guiWidget_t*)_widget)->isEnabled)
#define widget_isVisible(_widget) (((guiWidget_t*)_widget)->isVisible)
#define widget_isFocusable(_widget) \
	(((guiWidget_t*)_widget)->isFocusable && ((guiWidget_t*)_widget)->isEnabled \
	 && ((guiWidget_t*)_widget)->isVisible)
#define widget_isContainer(_widget) (((guiWidget_t*)_widget)->isContainer)
#define widget_setVisible(_widget, _state) { ((guiWidget_t*)_widget)->isVisible = _state; }
#define widget_setEnabled(_widget, _state) { ((guiWidget_t*)_widget)->isEnabled = _state; }
void _widget_setFocusable(guiWidget_t *widget, bool state);
#define widget_setFocusable(_widget, _state) _widget_setFocusable((guiWidget_t*)_widget, _state)

// Dimension management

void widget_setDimension(guiWidget_t *widget, const guiRectangle_t newDim);
void widget_getAbsolutePosition(guiWidget_t *widget, vec2i_t *x);

#define widget_setWidth(_widget, _width)         \
	{ \
		guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim; \
		newDimension.width = (_width);                \
		widget_setDimension(((guiWidget_t*)_widget), newDimension); \
	}

#define widget_setHeight(_widget, _height)       \
	{ \
		guiRectangle_t newDimension = ((guiWidget_t*)_widget)->dim; \
		newDimension.height = (_height);              \
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
		newDimension.pos = (_pos);                     \
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


/**
 * @brief Handles the event by widget
 *
 * Distributes the event to appropriate event listeners in widget. All events should use this method
 * to handle themselves. 
 *
 * @param widget Widget which will handle the event
 * @param event Event to handle
 * @param event Should event be freed after handling
 */
void widget_handleEvent(guiWidget_t *widget, event_t *event, bool freeEvent);

#endif // WIDGET_H_INCLUDED
