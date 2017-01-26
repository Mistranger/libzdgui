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
	const char* (*w_typename)(struct guiScrollArea_s *widget);
	void (*w_destructor)(struct guiScrollArea_s *widget);
	struct guiRectangle_s* (*w_getChildrenArea)(const struct guiScrollArea_s* scrollarea);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiScrollArea_s* widget, vec2i_t pos);
	void(*w_draw)(const struct guiScrollArea_s* scrollarea, guiGraphics_t* graphics);
	void(*w_tick)(struct guiScrollArea_s* scrollarea);
	bool(*w_isWidgetExisting)(struct guiWidget_s* widget, const struct guiWidget_s* exist);
	void(*w_setFocusHandler)(struct guiContainer_s *widget, void *focus);
	
	void (*c_showWidgetPart)(struct guiScrollArea_s *container, guiWidget_t *widget, guiRectangle_t area);
} guiScrollArea_vf_t;

typedef struct guiScrollArea_s {
	guiContainer_t widget;

	bool hBarVisible, vBarVisible;
	int scrollBarWidth;
	vec2i_t scrollPos;
	ScrollAreaPolicy_t hPolicy, vPolicy;
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
	guiImage_t *upButtonImage;
	guiImage_t *upPressedButtonImage;
	guiImage_t *downButtonImage;
	guiImage_t *downPressedButtonImage;
	guiImage_t *leftButtonImage;
	guiImage_t *leftPressedButtonImage;
	guiImage_t *rightButtonImage;
	guiImage_t *rightPressedButtonImage;
	guiImage_t *hBarImage;
	guiImage_t *vBarImage;
	guiImage_t *markerImage;
} guiScrollArea_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

guiScrollArea_t* scroll_new(guiGUI_t *gui, guiWidget_t *content);
void scroll_init(guiScrollArea_t* scrollarea, guiWidget_t *content);
static void scroll_destructor(guiScrollArea_t *scrollarea);

// Virtual inherited from guiWidget_t
const char* scroll_typename(guiScrollArea_t *widget);
guiRectangle_t* scroll_getChildrenArea(const guiScrollArea_t* scrollarea);
guiWidget_t* scroll_getWidgetAt(const guiScrollArea_t *scrollarea, vec2i_t pos);
void scroll_draw(const guiScrollArea_t* scrollarea, guiGraphics_t* graphics);
void scroll_tick(guiScrollArea_t *scrollarea);

#define scroll_getContent(_widget) \
	(list_size(((guiContainer_t*)_widget)->children) > 0 ? ((guiWidget_t*)list_front(((guiContainer_t*)_widget)->children)->data) : NULL)
void scroll_setContent(guiScrollArea_t* scrollarea, guiWidget_t *content);
guiRectangle_t* scroll_getContentDimension(const guiScrollArea_t* scrollarea);
void scroll_showWidgetPart(guiScrollArea_t* scrollarea, guiWidget_t *widget, guiRectangle_t area);

// Scroll bars
void scroll_checkPolicies(guiScrollArea_t* scrollarea);
int scroll_getHorizontalMaxScroll(const guiScrollArea_t* scrollarea);
int scroll_getVerticalMaxScroll(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getVerticalBarDimension(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getHorizontalBarDimension(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getUpButtonDimension(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getDownButtonDimension(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getLeftButtonDimension(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getRightButtonDimension(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getVerticalMarkerDimension(const guiScrollArea_t* scrollarea);
guiRectangle_t* scroll_getHorizontalMarkerDimension(const guiScrollArea_t* scrollarea);
#define scroll_getScrollbarWidth(_widget) (((guiScrollArea_t*)_widget)->scrollBarWidth)
#define scroll_setScrollbarWidth(_widget, _width) { ((guiScrollArea_t*)_widget)->scrollBarWidth = _width; }
#define scroll_getHorizontalScrollAmount(_widget) (((guiScrollArea_t*)_widget)->scrollPos.x)
#define scroll_getVerticalScrollAmount(_widget) (((guiScrollArea_t*)_widget)->scrollPos.y)
#define scroll_setHorizontalScrollAmount(_widget, _amount) {            \
	int max = scroll_getHorizontalMaxScroll((guiScrollArea_t*)_widget); \
	((guiScrollArea_t*)_widget)->scrollPos.x = _amount;                 \
	clamp(((guiScrollArea_t*)_widget)->scrollPos.x, 0, max);            \
 }
#define scroll_setVerticalScrollAmount(_widget, _amount) {              \
	int max = scroll_getVerticalMaxScroll((guiScrollArea_t*)_widget);   \
	((guiScrollArea_t*)_widget)->scrollPos.y = _amount;                 \
	clamp(((guiScrollArea_t*)_widget)->scrollPos.y, 0, max);            \
 }
#define scroll_setScrollAmount(_hScroll, _vScroll) {                    \
	setHorizontalScrollAmount(_hScroll);                                \
	setVerticalScrollAmount(_vScroll);                                  \
}

#define scroll_getLeftButtonScrollAmount(_widget) (((guiScrollArea_t*)_widget)->leftButtonScrollAmount)
#define scroll_setLeftButtonScrollAmount(_widget, _amount) { ((guiScrollArea_t*)_widget)->leftButtonScrollAmount = _amount; }
#define scroll_getRightButtonScrollAmount(_widget) (((guiScrollArea_t*)_widget)->rightButtonScrollAmount)
#define scroll_setRightButtonScrollAmount(_widget, _amount) { ((guiScrollArea_t*)_widget)->rightButtonScrollAmount = _amount; }
#define scroll_getUpButtonScrollAmount(_widget) (((guiScrollArea_t*)_widget)->upButtonScrollAmount)
#define scroll_setUpButtonScrollAmount(_widget, _amount) { ((guiScrollArea_t*)_widget)->upButtonScrollAmount = _amount; }
#define scroll_getDownButtonScrollAmount(_widget) (((guiScrollArea_t*)_widget)->downButtonScrollAmount)
#define scroll_setDownButtonScrollAmount(_widget, _amount) { ((guiScrollArea_t*)_widget)->downButtonScrollAmount = _amount; }

// Image getters /setters macros
#define scroll_getUpButtonImage(_widget) (((guiScrollArea_t*)_widget)->upButtonImage)
#define scroll_setUpButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->upButtonImage = _image; }
#define scroll_getUpPressedButtonImage(_widget) (((guiScrollArea_t*)_widget)->upPressedButtonImage)
#define scroll_setUpPressedButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->upPressedButtonImage = _image; }
#define scroll_getDownButtonImage(_widget) (((guiScrollArea_t*)_widget)->downButtonImage)
#define scroll_setDownButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->downButtonImage = _image; }
#define scroll_getDownPressedButtonImage(_widget) (((guiScrollArea_t*)_widget)->downPressedButtonImage)
#define scroll_setDownPressedButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->downPressedButtonImage = _image; }
#define scroll_getLeftButtonImage(_widget) (((guiScrollArea_t*)_widget)->leftButtonImage)
#define scroll_setLeftButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->leftButtonImage = _image; }
#define scroll_getLeftPressedButtonImage(_widget) (((guiScrollArea_t*)_widget)->leftPressedButtonImage)
#define scroll_setLeftPressedButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->leftPressedButtonImage = _image; }
#define scroll_getRightButtonImage(_widget) (((guiScrollArea_t*)_widget)->rightButtonImage)
#define scroll_setRightButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->rightButtonImage = _image; }
#define scroll_getRightPressedButtonImage(_widget) (((guiScrollArea_t*)_widget)->rightPressedButtonImage)
#define scroll_setRightPressedButtonImage(_widget, _image) { ((guiScrollArea_t*)_widget)->rightPressedButtonImage = _image; }
#define scroll_getHBarImage(_widget) (((guiScrollArea_t*)_widget)->hBarImage)
#define scroll_setHBarImage(_widget, _image) { ((guiScrollArea_t*)_widget)->hBarImage = _image; }
#define scroll_getVBarImage(_widget) (((guiScrollArea_t*)_widget)->vBarImage)
#define scroll_setVBarImage(_widget, _image) { ((guiScrollArea_t*)_widget)->vBarImage = _image; }
#define scroll_getMarkerImage(_widget) (((guiScrollArea_t*)_widget)->markerImage)
#define scroll_setMarkerImage(_widget, _image) { ((guiScrollArea_t*)_widget)->markerImage = _image; }

// Event listeners
void scroll_mousePressed(void *widget, mouseEvent_t *mouseEvent);
void scroll_mouseReleased(void *widget, mouseEvent_t *mouseEvent);
void scroll_resized(void *widget, dimensionEvent_t *dimensionEvent);

#endif // WIDGETS_SCROLLAREA_H_INCLUDED
