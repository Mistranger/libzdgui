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

// Window flags
typedef enum WindowFlags {
    WF_CANDRAG = 0x00000001,
    WF_SIZABLE = 0x00000002,
    WF_HASTITLE = 0x00000004,
} WindowFlags_t;

typedef enum WindowButtons {
	WB_CLOSE = 0x00000001,
	WB_MINIMIZE = 0x00000002,
} WindowButtons_t;

struct guiWindow_s;

typedef struct guiWindow_vf {
	const char* (*w_typename)(struct guiWindow_s *widget);
	void (*w_destructor)(struct guiWindow_s *widget);
    struct guiRectangle_s* (*w_getChildrenArea)(const struct guiWindow_s* window);
    struct guiWidget_s* (*w_getWidgetAt)(const struct guiContainer_s* widget, vec2i_t pos);
    void (*w_draw)(const struct guiWindow_s* window, guiGraphics_t* graphics);
    void (*w_tick)(struct guiContainer_s* widget);
    bool (*w_isWidgetExisting)(struct guiContainer_s* widget, const struct guiWidget_s* exist);
	void(*w_setFocusHandler)(struct guiContainer_s *widget, void *focus);
} guiWindow_vf_t;

typedef struct guiWindow_s {
    guiContainer_t widget;
    string_t *caption;
    WindowFlags_t windowFlags;
	WindowButtons_t windowButtons;
    size_t padding;
    size_t titleBarHeight;
	guiImage_t *background;
} guiWindow_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

#define window_frontItem(window) (list_front(window->widget.children))

/**
 * @brief              Constructor (window initialization with caption)
 * @param caption      title caption
 */
guiWindow_t* window_new(guiGUI_t *gui, const string_t *caption, guiImage_t *background);
void window_init(guiWindow_t* window, const string_t *caption, guiImage_t *background);
void window_destructor(guiWindow_t *window);

// Virtual inherited from guiWidget_t

const char* window_typename(guiWindow_t *widget);
guiRectangle_t* window_getChildrenArea(const guiWindow_t* window);
void window_draw(const guiWindow_t* window, guiGraphics_t* graphics);

/**
 * @brief				Sets windows title caption
 * @param window
 * @param caption
 */
void window_setCaption(guiWindow_t* window, const string_t *caption);

/**
 * @brief 				Gets window's title caption
 * @param window
 */
string_t* window_getCaption(const guiWindow_t* window);

#define window_getPadding(_window) (window->padding)
#define window_setPadding(_window, _padding) { _window->padding = _padding; }

void window_resizeToContent(const guiWindow_t* window);

// Event listeners

void window_mousePressed(void* widget, mouseEvent_t* mouseEvent);
void window_mouseReleased(void* widget, mouseEvent_t* mouseEvent);
void window_resized(void *widget, dimensionEvent_t *dimEvent);

#endif // WIDGETS_WINDOW_H_INCLUDED
