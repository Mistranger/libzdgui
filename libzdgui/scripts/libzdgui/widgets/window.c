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

const char *window_typename(guiWindow *widget)
{
	return WindowType;
}

guiWindow *window_new(guiGUI *gui, const string_t *caption, guiImage *background)
{
	guiWindow *window = new (guiWindow);
	window_init(window, caption, background);
	gui_addWidget(gui, window);
	return window;
}

void window_destructor(guiWindow *window)
{
	string_delete(window->caption);
	widget_destructor((guiWidget *)window);
}

void window_init(guiWindow *window, const string_t *caption, guiImage *background)
{
	container_init((guiContainer *)window);
	((guiWidget *)(&window->widget))->v = (guiWidget_vf_t *)&guiWindow_vtable;

	window->caption = string_new();
	window_setCaption(window, caption);
	window->canDrag = true;
	window->isSizable = true;
	window->hasTitleBar = false;
	
	window->newDim = (guiRectangle){0, 0, 0, 0};
	window->isHorizontalRightResizing = false;
	window->isVerticalRightResizing = false;
	window->isHorizontalLeftResizing = false;
	window->isVerticalLeftResizing = false;
	window_setPadding(window, 0);
	window->titleBarHeight = 16;
	window->background = background;
	
	window->isDragging = false;
	window->dragOffset = (vec2i){0, 0};

	widget_addMouseListener(&window->widget, ME_PRESSED, window_mousePressed);
	widget_addMouseListener(&window->widget, ME_DRAGGED, window_mouseDragged);
	widget_addMouseListener(&window->widget, ME_MOVED, window_mouseMoved);
	widget_addMouseListener(&window->widget, ME_RELEASED, window_mouseReleased);
	widget_addDimensionListener(&window->widget, DE_RESIZED, window_resized);
}

string_t *window_getCaption(const guiWindow *window)
{
	return window->caption;
}

void window_setCaption(guiWindow *window, const string_t *caption)
{
	string_copy(window->caption, caption, 0, string_size(caption));
}

void window_resizeToContent(const guiWindow *window)
{
	int w = 0, h = 0;
	guiWidget *widget;
	for (listNode_t *node = window_frontItem(window); node; node = node->next) {
		widget = (guiWidget *)node->data;
		if (widget_getX(widget) + widget_getWidth(widget) > w) {
			w = widget_getX(widget) + widget_getWidth(widget);
		}

		if (widget_getY(widget) + widget_getHeight(widget) > h) {
			h = widget_getY(widget) + widget_getHeight(widget);
		}
	}
	widget_setSize(window, w + 2 * window_getPadding(window), h + window_getPadding(window));
}

void window_draw(const guiWindow *window, guiGraphics *graphics)
{
	if (window->background) {
		graph_drawImageScaled(graphics, 0, 0, image_getWidth(*window->background), image_getHeight(*window->background),
							  widget_getWidth(window), widget_getHeight(window), image_getImage(*window->background));
	}
	container_draw((guiContainer *)window, graphics);
	if (window->canDrag && window->isDragging) {
		graph_drawRectAlpha(graphics, window->newDim.pos.x,  window->newDim.pos.y, 
			widget_getWidth(window), widget_getHeight(window), s"Cyan", 0.5);
	} else if (window->isSizable && (window->isHorizontalRightResizing || window->isHorizontalLeftResizing 
		|| window->isVerticalRightResizing || window->isVerticalLeftResizing)) {
			guiRectangle newD = {0, 0, widget_getWidth(window), widget_getHeight(window)};
			if (window->isHorizontalRightResizing) {
				newD.width = MAX(16, widget_getWidth(window) + window->newDim.width);
			}
			if (window->isHorizontalLeftResizing) {
				newD.pos.x = window->newDim.pos.x;
				newD.width = MAX(16, widget_getWidth(window) + window->newDim.width);
			}
			if (window->isVerticalRightResizing) {
				newD.height =  MAX(16, widget_getHeight(window) + window->newDim.height);
			}
			if (window->isVerticalLeftResizing) {
				newD.pos.y = window->newDim.pos.y;
				newD.height = MAX(16, widget_getHeight(window) + window->newDim.height);
			}
			graph_drawRectAlpha(graphics, newD.pos.x,  newD.pos.y, 
				newD.width, newD.height, s"Cyan", 0.5);
	}
	
}

guiRectangle *window_getChildrenArea(const guiWindow *window)
{
	return &(guiRectangle) {
		window_getPadding(window), 0,
			widget_getWidth(window) - 2 * window_getPadding(window), widget_getHeight(window) - window_getPadding(window)
	};
}

void window_mousePressed(void *widget, guiMouseEvent *mouseEvent)
{
	if (event_getSource(mouseEvent) != widget) {
		return;
	}
	
	guiWindow *window = (guiWindow *)widget;
	if (widget_getParent(widget) != NULL) {
		container_moveToTop((guiContainer *)widget_getParent(widget), (guiWidget *)widget);
	}
	window->newDim = (guiRectangle){0, 0, 0, 0};
	
	if (window->canDrag && mouseEvent->pos.y >= 5 && mouseEvent->pos.y <= window->titleBarHeight) {
		window->isDragging = true;
		window->dragOffset = (vec2i){mouseEvent->pos.x, mouseEvent->pos.y};
	} else if (window->isSizable) {
		if (abs(widget_getWidth(window) - 1 - mouseEvent->pos.x) < 5) {
			window->isHorizontalRightResizing = true;
			window->resizePos.x = mouseEvent->pos.x;
		} else if (abs(mouseEvent->pos.x) < 5) {
			
			window->isHorizontalLeftResizing = true;
			window->resizePos.x = mouseEvent->pos.x;
		}
		if (abs(widget_getHeight(window) - 1 - mouseEvent->pos.y) < 5) {
			window->isVerticalRightResizing = true;
			window->resizePos.y = mouseEvent->pos.y;
		} else if (abs(mouseEvent->pos.y) < 5) {
			window->isVerticalLeftResizing = true;
			window->resizePos.y = mouseEvent->pos.y;
		}
	}
}

void window_mouseMoved(void *widget, guiMouseEvent *mouseEvent)
{
	
}

void window_mouseDragged(void *widget, guiMouseEvent *mouseEvent)
{
	guiWindow *window = (guiWindow *)widget;
	if (window->canDrag && window->isDragging) {
		window->newDim.pos.x =  mouseEvent->pos.x - window->dragOffset.x;
		window->newDim.pos.y =  mouseEvent->pos.y - window->dragOffset.y;
	}
	if (window->isSizable && (window->isHorizontalRightResizing || window->isHorizontalLeftResizing 
		|| window->isVerticalRightResizing || window->isVerticalLeftResizing)) {
			if (window->isHorizontalRightResizing) {
				window->newDim.width = mouseEvent->pos.x - window->resizePos.x;
			}
			if (window->isHorizontalLeftResizing) {
				window->newDim.pos.x = mouseEvent->pos.x - window->resizePos.x;
				window->newDim.width = -(mouseEvent->pos.x - window->resizePos.x);
			}
			if (window->isVerticalRightResizing) {
				window->newDim.height = mouseEvent->pos.y - window->resizePos.y;
			}
			if (window->isVerticalLeftResizing) {
				window->newDim.pos.y = mouseEvent->pos.y - window->resizePos.y;
				window->newDim.height = -(mouseEvent->pos.y - window->resizePos.y);
			}
	}
}

void window_mouseReleased(void *widget, guiMouseEvent *mouseEvent)
{
	guiWindow *window = (guiWindow *)widget;
	if (window->canDrag && window->isDragging) {
		window->isDragging = false;
		window->newDim.pos = (vec2i){widget_getX(window) + window->newDim.pos.x,  widget_getY(window) + window->newDim.pos.y};
		widget_setPosition(window, window->newDim.pos);
	} else if (window->isSizable && (window->isHorizontalRightResizing || window->isHorizontalLeftResizing 
		|| window->isVerticalRightResizing || window->isVerticalLeftResizing)) {
		guiRectangle newD = widget_getDimensions(window);
		if (window->isHorizontalRightResizing) {
			newD.width = MAX(16, widget_getWidth(window) + window->newDim.width);
		}
		if (window->isHorizontalLeftResizing) {
			newD.pos.x = widget_getX(window) + window->newDim.pos.x;
			newD.width = MAX(16, widget_getWidth(window) + window->newDim.width);
		}
		if (window->isVerticalRightResizing) {
			newD.height =  MAX(16, widget_getHeight(window) + window->newDim.height);
		}
		if (window->isVerticalLeftResizing) {
			newD.pos.y = widget_getY(window) + window->newDim.pos.y;
			newD.height = MAX(16, widget_getHeight(window) + window->newDim.height);
		}
		window->isHorizontalRightResizing = false;
		window->isVerticalRightResizing = false;
		window->isHorizontalLeftResizing = false;
		window->isVerticalLeftResizing = false;
		widget_setDimension(widget, newD);
	}
}

void window_resized(void *widget, guiDimensionEvent *dimEvent)
{
	guiInfo("window resized, new size %d, %d" _C_ dimEvent->width _C_ dimEvent->height);
}

