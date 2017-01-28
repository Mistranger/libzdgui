#include "system.h"

#include "widgets/window.h"

guiWindow_vf_t guiWindow_vtable = {
	window_typename,
	window_destructor,
	window_getChildrenArea,
	container_getWidgetAt,
	window_draw,
	container_tick,
	container_isWidgetExisting,
	container_setFocusManager
};

const char *WindowType = "Window";

const char *window_typename(guiWindow_t *widget)
{
	return WindowType;
}

guiWindow_t *window_new(guiGUI_t *gui, const string_t *caption, guiImage_t *background)
{
	guiWindow_t *window = new (guiWindow_t);
	window_init(window, caption, background);
	gui_addWidget(gui, window);
	return window;
}

void window_destructor(guiWindow_t *window)
{
	string_delete(window->caption);
	widget_destructor((guiWidget_t *)window);
}

void window_init(guiWindow_t *window, const string_t *caption, guiImage_t *background)
{
	container_init((guiContainer_t *)window);
	((guiWidget_t *)(&window->widget))->v = (guiWidget_vf_t *)&guiWindow_vtable;

	window->caption = string_new();
	window_setCaption(window, caption);
	window->canDrag = false;
	window->isSizable = false;
	window->hasTitleBar = false;
	window->isDragging = false;
	window_setPadding(window, 0);
	window->background = background;

	widget_addMouseListener(&window->widget, ME_PRESSED, window_mousePressed);
	widget_addMouseListener(&window->widget, ME_MOVED, window_mouseMoved);
	widget_addMouseListener(&window->widget, ME_RELEASED, window_mouseReleased);
	widget_addDimensionListener(&window->widget, DE_RESIZED, window_resized);
}

string_t *window_getCaption(const guiWindow_t *window)
{
	return window->caption;
}

void window_setCaption(guiWindow_t *window, const string_t *caption)
{
	string_copy(window->caption, caption, 0, string_size(caption));
}

void window_resizeToContent(const guiWindow_t *window)
{
	int w = 0, h = 0;
	guiWidget_t *widget;
	for (listNode_t *node = window_frontItem(window); node; node = node->next) {
		widget = (guiWidget_t *)node->data;
		if (widget_getX(widget) + widget_getWidth(widget) > w) {
			w = widget_getX(widget) + widget_getWidth(widget);
		}

		if (widget_getY(widget) + widget_getHeight(widget) > h) {
			h = widget_getY(widget) + widget_getHeight(widget);
		}
	}
	widget_setSize(window, w + 2 * window_getPadding(window), h + window_getPadding(window));
}

void window_draw(const guiWindow_t *window, guiGraphics_t *graphics)
{
	if (window->background) {
		graph_drawImageScaled(graphics, 0, 0, image_getWidth(*window->background), image_getHeight(*window->background),
							  widget_getWidth(window), widget_getHeight(window), image_getImage(*window->background));
	}
	container_draw((guiContainer_t *)window, graphics);
}

guiRectangle_t *window_getChildrenArea(const guiWindow_t *window)
{
	return &(guiRectangle_t) {
		window_getPadding(window), 0,
						  widget_getWidth(window) - 2 * window_getPadding(window), widget_getHeight(window) - window_getPadding(window)
	};
}

void window_mousePressed(void *widget, mouseEvent_t *mouseEvent)
{
	guiWindow_t *window = (guiWindow_t *)widget;
	if (widget_getParent(widget) != NULL) {
		container_moveToTop((guiContainer_t *)widget_getParent(widget), (guiWidget_t *)widget);
	}
	if (window->isSizable) {
		vec2i_t absPos;
		widget_getAbsolutePosition(widget, &absPos);
		if (absPos.x + widget_getWidth(window) - 1 == mouseEvent->pos.x) {
			window->isDragging = true;
		}
	}
}

void window_mouseMoved(void *widget, mouseEvent_t *mouseEvent)
{
	guiWindow_t *window = (guiWindow_t *)widget;
	if (window->isSizable && window->isDragging) {

	}
}

void window_mouseReleased(void *widget, mouseEvent_t *mouseEvent)
{
	guiWindow_t *window = (guiWindow_t *)widget;
	if (window->isSizable) {
		if (window->isDragging) {

		}

	}
}

void window_resized(void *widget, dimensionEvent_t *dimEvent)
{
	guiInfo("window resized, new size %d, %d" _C_ dimEvent->width _C_ dimEvent->height);
}

