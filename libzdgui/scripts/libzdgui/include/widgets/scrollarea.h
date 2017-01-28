#ifndef WIDGETS_SCROLLAREA_H_INCLUDED
#define WIDGETS_SCROLLAREA_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "util/string.h"
#include "event/event_mouse.h"
#include "event/event_dimension.h"
#include "widgets/container.h"
#include "dimension.h"
#include "gui.h"
#include "image.h"
#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef enum ScrollAreaPolicy {
	SAP_AUTO = 0,
	SAP_ALWAYS,
	SAP_NEVER
} ScrollAreaPolicy_t;

extern const char *ScrollAreaType;

typedef struct guiScrollArea_vf {
	const char *(*w_typename)(struct guiScrollArea *widget);
	void (*w_destructor)(struct guiScrollArea *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiScrollArea *scrollarea);
	struct guiWidget *(*w_getWidgetAt)(const struct guiScrollArea *widget, vec2i pos);
	void(*w_draw)(const struct guiScrollArea *scrollarea, guiGraphics *graphics);
	void(*w_tick)(struct guiScrollArea *scrollarea);
	bool(*w_isWidgetExisting)(struct guiContainer *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiContainer *widget, void *focus);

	void (*c_showWidgetPart)(struct guiScrollArea *container, guiWidget *widget, guiRectangle area);
} guiScrollArea_vf_t;

typedef struct guiScrollArea {
	guiContainer widget;

	bool hBarVisible, vBarVisible;
	int scrollBarWidth;
	vec2i scrollPos;
	ScrollAreaPolicy_t hPolicy, vPolicy;
	dimensionListener_t *dimListener;
	guiLifecycleListener *lifecycleListener;
	int upButtonScrollAmount;
	int downButtonScrollAmount;
	int leftButtonScrollAmount;
	int rightButtonScrollAmount;
	bool upButtonPressed;
	bool downButtonPressed;
	bool leftButtonPressed;
	bool rightButtonPressed;
	bool isVerticalMarkerDragged;
	bool isHorizontalMarkerDragged;
	int horizontalMarkerDragOffset;
	int verticalMarkerDragOffset;

	// Images
	guiImage *upButtonImage;
	guiImage *upPressedButtonImage;
	guiImage *downButtonImage;
	guiImage *downPressedButtonImage;
	guiImage *leftButtonImage;
	guiImage *leftPressedButtonImage;
	guiImage *rightButtonImage;
	guiImage *rightPressedButtonImage;
	guiImage *hBarImage;
	guiImage *vBarImage;
	guiImage *markerImage;
} guiScrollArea;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

guiScrollArea *scroll_new(guiGUI *gui, guiWidget *content);
void scroll_init(guiScrollArea *scrollarea, guiWidget *content);
void scroll_destructor(guiScrollArea *scrollarea);
void scroll_death(void *widget, guiLifecycleEvent *event);

// Virtual inherited from guiWidget
const char *scroll_typename(guiScrollArea *widget);
guiRectangle *scroll_getChildrenArea(const guiScrollArea *scrollarea);
guiWidget *scroll_getWidgetAt(const guiScrollArea *scrollarea, vec2i pos);
void scroll_draw(const guiScrollArea *scrollarea, guiGraphics *graphics);
void scroll_tick(guiScrollArea *scrollarea);

#define scroll_getContent(_widget) \
	(list_size(((guiContainer*)_widget)->children) > 0 ? ((guiWidget*)list_front(((guiContainer*)_widget)->children)->data) : NULL)
void scroll_setContent(guiScrollArea *scrollarea, guiWidget *content);
guiRectangle *scroll_getContentDimension(const guiScrollArea *scrollarea);
void scroll_showWidgetPart(guiScrollArea *scrollarea, guiWidget *widget, guiRectangle area);

// Scroll bars
void scroll_checkPolicies(guiScrollArea *scrollarea);
int scroll_getHorizontalMaxScroll(const guiScrollArea *scrollarea);
int scroll_getVerticalMaxScroll(const guiScrollArea *scrollarea);
guiRectangle *scroll_getVerticalBarDimension(const guiScrollArea *scrollarea);
guiRectangle *scroll_getHorizontalBarDimension(const guiScrollArea *scrollarea);
guiRectangle *scroll_getUpButtonDimension(const guiScrollArea *scrollarea);
guiRectangle *scroll_getDownButtonDimension(const guiScrollArea *scrollarea);
guiRectangle *scroll_getLeftButtonDimension(const guiScrollArea *scrollarea);
guiRectangle *scroll_getRightButtonDimension(const guiScrollArea *scrollarea);
guiRectangle *scroll_getVerticalMarkerDimension(const guiScrollArea *scrollarea);
guiRectangle *scroll_getHorizontalMarkerDimension(const guiScrollArea *scrollarea);
#define scroll_getScrollbarWidth(_widget) (((guiScrollArea*)_widget)->scrollBarWidth)
#define scroll_setScrollbarWidth(_widget, _width) { ((guiScrollArea*)_widget)->scrollBarWidth = _width; }
#define scroll_getHorizontalScrollAmount(_widget) (((guiScrollArea*)_widget)->scrollPos.x)
#define scroll_getVerticalScrollAmount(_widget) (((guiScrollArea*)_widget)->scrollPos.y)
#define scroll_setHorizontalScrollAmount(_widget, _amount) {            \
		int max = scroll_getHorizontalMaxScroll((guiScrollArea*)_widget); \
		((guiScrollArea*)_widget)->scrollPos.x = _amount;                 \
		clamp(((guiScrollArea*)_widget)->scrollPos.x, 0, max);            \
	}
#define scroll_setVerticalScrollAmount(_widget, _amount) {              \
		int max = scroll_getVerticalMaxScroll((guiScrollArea*)_widget);   \
		((guiScrollArea*)_widget)->scrollPos.y = _amount;                 \
		clamp(((guiScrollArea*)_widget)->scrollPos.y, 0, max);            \
	}
#define scroll_setScrollAmount(_hScroll, _vScroll) {                    \
		setHorizontalScrollAmount(_hScroll);                                \
		setVerticalScrollAmount(_vScroll);                                  \
	}

#define scroll_getLeftButtonScrollAmount(_widget) (((guiScrollArea*)_widget)->leftButtonScrollAmount)
#define scroll_setLeftButtonScrollAmount(_widget, _amount) { ((guiScrollArea*)_widget)->leftButtonScrollAmount = _amount; }
#define scroll_getRightButtonScrollAmount(_widget) (((guiScrollArea*)_widget)->rightButtonScrollAmount)
#define scroll_setRightButtonScrollAmount(_widget, _amount) { ((guiScrollArea*)_widget)->rightButtonScrollAmount = _amount; }
#define scroll_getUpButtonScrollAmount(_widget) (((guiScrollArea*)_widget)->upButtonScrollAmount)
#define scroll_setUpButtonScrollAmount(_widget, _amount) { ((guiScrollArea*)_widget)->upButtonScrollAmount = _amount; }
#define scroll_getDownButtonScrollAmount(_widget) (((guiScrollArea*)_widget)->downButtonScrollAmount)
#define scroll_setDownButtonScrollAmount(_widget, _amount) { ((guiScrollArea*)_widget)->downButtonScrollAmount = _amount; }

// Image getters /setters macros
#define scroll_getUpButtonImage(_widget) (((guiScrollArea*)_widget)->upButtonImage)
#define scroll_setUpButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->upButtonImage = _image; }
#define scroll_getUpPressedButtonImage(_widget) (((guiScrollArea*)_widget)->upPressedButtonImage)
#define scroll_setUpPressedButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->upPressedButtonImage = _image; }
#define scroll_getDownButtonImage(_widget) (((guiScrollArea*)_widget)->downButtonImage)
#define scroll_setDownButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->downButtonImage = _image; }
#define scroll_getDownPressedButtonImage(_widget) (((guiScrollArea*)_widget)->downPressedButtonImage)
#define scroll_setDownPressedButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->downPressedButtonImage = _image; }
#define scroll_getLeftButtonImage(_widget) (((guiScrollArea*)_widget)->leftButtonImage)
#define scroll_setLeftButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->leftButtonImage = _image; }
#define scroll_getLeftPressedButtonImage(_widget) (((guiScrollArea*)_widget)->leftPressedButtonImage)
#define scroll_setLeftPressedButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->leftPressedButtonImage = _image; }
#define scroll_getRightButtonImage(_widget) (((guiScrollArea*)_widget)->rightButtonImage)
#define scroll_setRightButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->rightButtonImage = _image; }
#define scroll_getRightPressedButtonImage(_widget) (((guiScrollArea*)_widget)->rightPressedButtonImage)
#define scroll_setRightPressedButtonImage(_widget, _image) { ((guiScrollArea*)_widget)->rightPressedButtonImage = _image; }
#define scroll_getHBarImage(_widget) (((guiScrollArea*)_widget)->hBarImage)
#define scroll_setHBarImage(_widget, _image) { ((guiScrollArea*)_widget)->hBarImage = _image; }
#define scroll_getVBarImage(_widget) (((guiScrollArea*)_widget)->vBarImage)
#define scroll_setVBarImage(_widget, _image) { ((guiScrollArea*)_widget)->vBarImage = _image; }
#define scroll_getMarkerImage(_widget) (((guiScrollArea*)_widget)->markerImage)
#define scroll_setMarkerImage(_widget, _image) { ((guiScrollArea*)_widget)->markerImage = _image; }

// Event listeners
void scroll_mousePressed(void *widget, guiMouseEvent *mouseEvent);
void scroll_mouseDragged(void *widget, guiMouseEvent *mouseEvent);
void scroll_mouseReleased(void *widget, guiMouseEvent *mouseEvent);
void scroll_resized(void *widget, guiDimensionEvent *dimensionEvent);

#endif // WIDGETS_SCROLLAREA_H_INCLUDED
