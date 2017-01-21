#ifndef WIDGETS_WINDOW_H_INCLUDED
#define WIDGETS_WINDOW_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "widgets/container.h"
#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

// Maximum length of window's caption
#define MAX_CAPTION_LENGTH 256

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

// Window flags
enum WindowFlags
{
	WF_CANDRAG = 0x00000001,
};

struct guiWindow_s;

typedef struct guiWindow_vf {
	
	struct guiRectangle_s (*w_getChildrenArea)(const struct guiWindow_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiContainer_s *widget, int x, int y);
	void (*w_draw)(const struct guiWindow_s *container, guiGraphics_t *graphics);
	void (*w_tick)(struct guiContainer_s *widget);
	bool (*w_isWidgetExisting)(struct guiContainer_s *widget, const struct guiWidget_s *exist);
} guiWindow_vf_t;

typedef struct guiWindow_s
{
    guiContainer_t widget;
	char caption[MAX_CAPTION_LENGTH];
	enum WindowFlags windowFlags;
	size_t padding;
	
} guiWindow_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
 * @brief              Constructor (window initialization with caption)
 * @param caption      title caption
 */
guiWindow_t* window_new(const char *caption);
void window_init(guiWindow_t *window, const char *caption);


// Virtual inherited from guiWidget_t

guiRectangle_t window_getChildrenArea(const guiWindow_t *widget);
void window_draw(const guiWindow_t *widget, guiGraphics_t *graphics);

/**
 * @brief				Sets windows title caption
 * @param window
 * @param caption
 */
void window_setCaption(guiWindow_t *window, const char *caption);

/**
 * @brief 				Gets window's title caption
 * @param window
 */
const char* window_getCaption(const guiWindow_t *window);

// Event listeners

void window_mousePressed(void *widget, mouseEvent_t *mouseEvent);
void window_mouseReleased(void *widget, mouseEvent_t *mouseEvent);


#endif // WIDGETS_WINDOW_H_INCLUDED
