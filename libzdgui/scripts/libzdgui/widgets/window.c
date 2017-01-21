#include "system.h"

#include "widgets/window.h"

guiWindow_vf_t guiWindow_vtable = {
	window_getChildrenArea,
	container_getWidgetAt,
	window_draw,
	container_tick,
	container_isWidgetExisting
};

guiWindow_t* window_new(const char* caption)
{
	guiWindow_t *window = (guiWindow_t*)malloc(sizeof(guiWindow_t));
	window_init(window, caption);
	return window;
}

void window_init(guiWindow_t *window, const char *caption)
{
	container_init((guiContainer_t*)window);
	((guiWidget_t*)(&window->widget))->v = (guiWidget_vf_t*)&guiWindow_vtable;
	
	window->padding = 0;
	window_setCaption(window, caption);
	widget_addMouseListener(&window->widget, EV_Mouse, ME_PRESSED, window_mousePressed);
	widget_addMouseListener(&window->widget, EV_Mouse, ME_RELEASED, window_mouseReleased);
	
}

const char* window_getCaption(const guiWindow_t* window)
{
	return window->caption;
}

void window_setCaption(guiWindow_t* window, const char* caption)
{
	strncpy(window->caption, caption, MAX_CAPTION_LENGTH);
}

void window_draw(const guiWindow_t* widget, guiGraphics_t* graphics)
{
	
}

guiRectangle_t window_getChildrenArea(const guiWindow_t* widget)
{
	
}

void window_mousePressed(void* widget, mouseEvent_t* mouseEvent)
{
	if (widget_getParent(widget) != NULL) {
		
	}
}

void window_mouseReleased(void* widget, mouseEvent_t* mouseEvent)
{
}


