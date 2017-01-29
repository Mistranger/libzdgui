#ifndef WIDGETS_WINDOW_H_INCLUDED
#define WIDGETS_WINDOW_H_INCLUDED

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

extern const char *WindowType;

typedef enum WindowButtons {
	WB_CLOSE = 0x00000001,
	WB_MINIMIZE = 0x00000002,
} WindowButtons_t;

struct guiWindow;

typedef struct guiWindow_vf {
	const char *(*w_typename)(struct guiWindow *widget);
	void (*w_destructor)(struct guiWindow *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWindow *window);
	struct guiWidget *(*w_getWidgetAt)(const struct guiContainer *widget, vec2i pos);
	void (*w_draw)(const struct guiWindow *window, guiGraphics *graphics);
	void (*w_tick)(struct guiContainer *widget);
	bool (*w_isWidgetExisting)(struct guiContainer *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiContainer *widget, void *focus);
	struct vec2i* (*w_getMinimalSize)(const struct guiWidget *widget);
	void (*c_showWidgetPart)(struct guiContainer *container, guiWidget *widget, guiRectangle area);
} guiWindow_vf_t;

typedef struct guiWindow {
	guiContainer widget;
	string_t *caption;
	bool canDrag;
	bool isSizable;
	bool hasTitleBar;

	guiRectangle newDim;
	vec2i resizePos;
	bool isHorizontalRightResizing;
	bool isVerticalRightResizing;
	bool isHorizontalLeftResizing;
	bool isVerticalLeftResizing;
	
	bool isDragging;
	vec2i dragOffset;
	
	WindowButtons_t windowButtons;
	size_t padding;
	size_t titleBarHeight;
	guiImage *background;
	guiImage *foreground;
} guiWindow;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

#define window_frontItem(window) (list_front(window->widget.children))

/**
 * @brief              Constructor (window initialization with caption)
 * @param caption      title caption
 */
guiWindow *window_new(guiGUI *gui, const string_t *caption);
void window_init(guiWindow *window, const string_t *caption);
void window_destructor(guiWindow *window);

// Virtual inherited from guiWidget_t

const char *window_typename(guiWindow *widget);
guiRectangle *window_getChildrenArea(const guiWindow *window);
void window_draw(const guiWindow *window, guiGraphics *graphics);

/**
 * @brief				Sets windows title caption
 * @param window
 * @param caption
 */
void window_setCaption(guiWindow *window, const string_t *caption);

/**
 * @brief 				Gets window's title caption
 * @param window
 */
string_t *window_getCaption(const guiWindow *window);

#define window_getBackground(_window) (((guiWindow*)_window)->background)
#define window_setBackground(_window, _image) { ((guiWindow*)_window)->background = _image; }
#define window_getForeground(_window) (((guiWindow*)_window)->foreground)
#define window_setForeground(_window, _image) { ((guiWindow*)_window)->foreground = _image; }

#define window_getPadding(_window) (window->padding)
#define window_setPadding(_window, _padding) { _window->padding = _padding; }

void window_resizeToContent(const guiWindow *window);

// Event listeners

void window_mousePressed(void *widget, guiMouseEvent *mouseEvent);
void window_mouseReleased(void *widget, guiMouseEvent *mouseEvent);
void window_mouseMoved(void *widget, guiMouseEvent *mouseEvent);
void window_mouseDragged(void *widget, guiMouseEvent *mouseEvent);
void window_resized(void *widget, guiDimensionEvent *dimEvent);

#endif // WIDGETS_WINDOW_H_INCLUDED
