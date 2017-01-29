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
	const char *(*w_typename)(struct guiWidget *widget);
	void (*w_destructor)(struct guiWidget *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void (*w_draw)(const struct guiWidget *widget, guiGraphics *graphics);
	void (*w_tick)(struct guiWidget *widget);
	bool (*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void (*w_setFocusManager)(struct guiWidget *widget, void *focus);
	struct vec2i* (*w_getMinimalSize)(const struct guiWidget *widget);
} guiWidget_vf_t;


/**
 * @class guiWidget_s
 * @date 21/01/2017
 * @file widget.h
 * @brief Generic widget structure. Used as a base class by other widget types.
 */
typedef struct guiWidget {
	void *gui;                            /// GUI object (used for some system requests, e.g. cursor change)
	
	list_t *eventListeners;               /// List holding all listeners for widget.
	
	guiRectangle dim;                     /// Dimensions of widget (position and size)
	
	struct guiWidget *parent;             /// Parent widget (e.g. container)
	void *focusManager;                   /// Focus manager, which handles all focus events

	// Flags
	bool isContainer;                     /// Can contain other widgets
	bool isEnabled;                       /// Can handle events and interact with other widgets
	bool isFocusable;                     /// Handle focus gain/lose events (otherwise it won't handle other events)
	bool isVisible;                       /// Is visible on screen
	
	guiFont *font;                        /// Font used for drawing
	__str fontColor;                      /// Font color (from TEXTCOLO lump)
	
	guiWidget_vf_t *v;                    /// Virtual functions table
} guiWidget;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/



/**
 * @brief              Constructor (widget initialization)
 * @param widget       widget to be init
 */
void widget_init(guiWidget *widget);



/**
 * @brief Widget destructor
 *
 * Handles the correct destruction of the widget.
 * However, this method is for internal use only, instead, use guiGui object
 * to delete a widget.
 *
 * @param widget  Widget to delete
 */
void widget_destructor(guiWidget *widget);
const char *widget_typename(guiWidget *widget);

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
guiMouseListener *widget_addMouseListener(void *widget,
										 mouseEventType_t mouseType, void(*listenerFunc)(void *widget, guiMouseEvent *event));
dimensionListener_t *widget_addDimensionListener(void *widget,
												 guiDimensionEventType dimType, void(*listenerFunc)(void *widget, guiDimensionEvent *event));
guiLifecycleListener *widget_addLifeCycleListener(void *widget,
												 guiLifecycleEventType dimType, void(*listenerFunc)(void *widget, guiLifecycleEvent *event));
void widget_removeListener(void *widget, void *listener);

// Virtual functions
guiRectangle *widget_getChildrenArea(const guiWidget *widget);
guiWidget *widget_getWidgetAt(const guiWidget *widget, vec2i pos);
void widget_draw(const guiWidget *widget, guiGraphics *graphics);
void widget_tick(guiWidget *widget);
bool widget_isWidgetExisting(guiWidget *widget, const guiWidget *exist);
#define widget_getFocusManager(_widget) (((guiWidget*)_widget)->focusManager)
void widget_setFocusManager(guiWidget *widget, void *focus);
vec2i* widget_getMinimalSize(const guiWidget *widget);

void widget_requestFocus(guiWidget *widget);
void widget_requestModalFocus(guiWidget *widget);
void widget_requestModalMouseInputFocus(guiWidget *widget);
void widget_releaseModalFocus(guiWidget *widget);
void widget_releaseModalMouseInputFocus(guiWidget *widget);
bool widget_hasModalFocus(const guiWidget *widget);
bool widget_hasModalMouseInputFocus(const guiWidget *widget);

// System requests

// Flags
#define widget_isEnabled(_widget) (((guiWidget*)_widget)->isEnabled)
#define widget_isVisible(_widget) (((guiWidget*)_widget)->isVisible)
#define widget_isFocusable(_widget) \
	(((guiWidget*)_widget)->isFocusable && ((guiWidget*)_widget)->isEnabled \
	 && ((guiWidget*)_widget)->isVisible)
#define widget_isFocused(_widget) \
	((!((guiWidget*)_widget)->focusManager) ? false : focus_isFocused(!((guiWidget*)_widget)->focusManager, (guiWidget*)_widget))
#define widget_isContainer(_widget) (((guiWidget*)_widget)->isContainer)
#define widget_setVisible(_widget, _state) { ((guiWidget*)_widget)->isVisible = _state; }
#define widget_setEnabled(_widget, _state) { ((guiWidget*)_widget)->isEnabled = _state; }
void _widget_setFocusable(guiWidget *widget, bool state);
#define widget_setFocusable(_widget, _state) _widget_setFocusable((guiWidget*)_widget, _state)

// Dimension management

void widget_setDimension(guiWidget *widget, const guiRectangle newDim);
void widget_getAbsolutePosition(guiWidget *widget, vec2i *x);

#define widget_setWidth(_widget, _width)         \
	{ \
		guiRectangle newDimension = ((guiWidget*)_widget)->dim; \
		newDimension.width = (_width);                \
		widget_setDimension(((guiWidget*)_widget), newDimension); \
	}

#define widget_setHeight(_widget, _height)       \
	{ \
		guiRectangle newDimension = ((guiWidget*)_widget)->dim; \
		newDimension.height = (_height);              \
		widget_setDimension(((guiWidget*)_widget), newDimension); \
	}

#define widget_setX(_widget, _x)         \
	{ \
		guiRectangle newDimension = ((guiWidget*)_widget)->dim;  \
		newDimension.pos.x = _x;                     \
		widget_setDimension(((guiWidget*)_widget), newDimension);  \
	}

#define widget_setY(_widget, _y)       \
	{ \
		guiRectangle newDimension = ((guiWidget*)_widget)->dim;  \
		newDimension.pos.y = _y;                     \
		widget_setDimension(((guiWidget*)_widget), newDimension);  \
	}

#define widget_setPosition(_widget, _pos)       \
	{ \
		guiRectangle newDimension = ((guiWidget*)_widget)->dim;  \
		newDimension.pos = (_pos);                     \
		widget_setDimension(((guiWidget*)_widget), newDimension);  \
	}

#define widget_setSize(_widget, _width, _height)   \
	{ \
		guiRectangle newDimension = ((guiWidget*)_widget)->dim;  \
		newDimension.width = _width;                 \
		newDimension.height = _height;               \
		widget_setDimension(((guiWidget*)_widget), newDimension);  \
	}

#define widget_getWidth(_widget) (((guiWidget*)_widget)->dim.width)
#define widget_getHeight(_widget) (((guiWidget*)_widget)->dim.height)
#define widget_getX(_widget) (((guiWidget*)_widget)->dim.pos.x)
#define widget_getY(_widget) (((guiWidget*)_widget)->dim.pos.y)
#define widget_getDimensions(_widget) (((guiWidget*)_widget)->dim)

// Children widgets management

// Parent widget access
#define widget_getParent(_widget) (((guiWidget*)_widget)->parent)
#define widget_setParent(_widget, _newParent) { (guiWidget*)_widget->parent = (guiWidget*)_newParent; }

// Property access
#define widget_getFont(_widget) (((guiWidget*)_widget)->font)
#define widget_setFont(_widget, _font) { ((guiWidget*)_widget)->font = _font; }
#define widget_getFontColor(_widget) (((guiWidget*)_widget)->fontColor)
#define widget_setFontColor(_widget, _font) { ((guiWidget*)_widget)->fontColor = _font; }
#define widget_getListeners(_widget) (((guiWidget*)_widget)->eventListeners)


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
void widget_handleEvent(guiWidget *widget, guiEvent *event, bool freeEvent);

#endif // WIDGET_H_INCLUDED
