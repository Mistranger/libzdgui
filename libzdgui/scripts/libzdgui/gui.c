#include "system.h"

#include "gui.h"

#include "debug.h"
#include "event.h"
#include "graphics.h"
#include "widget.h"
#include "util/util.h"
#include "widgets/container.h"

void gui_init(guiGUI_t* gui)
{
	gui->top = NULL;
	gui->graphics = NULL;
}
guiWidget_t* gui_getTop(guiGUI_t* gui)
{
	return gui->top;
}


void gui_setTop(guiGUI_t* gui, guiWidget_t* newTop)
{
	gui->top = newTop;
}

guiGraphics_t* gui_getGraphics(guiGUI_t* gui)
{
	return gui->graphics;
}

void gui_setGraphics(guiGUI_t* gui, guiGraphics_t* newGraphics)
{
	gui->graphics = newGraphics;
}

void gui_draw(guiGUI_t* gui)
{
	guiAssert(gui->top);
	guiAssert(gui->graphics);
	
	if (!(gui->top->flags & WF_VISIBLE)) {
		return;
	}
	
	graph_beginDraw(gui->graphics);
	guiDebugPrint("drawing top widget");
	graph_pushClipArea(gui->graphics, gui->top->dim);
	gui->top->v->w_draw(gui->top, gui->graphics);
	graph_popClipArea(gui->graphics);
	graph_endDraw(gui->graphics);
}


void gui_tick(guiGUI_t* gui)
{
	guiDebugPrint("gui tick");
	if (!gui->top) {
		// FIXME error
		return;
	}
	if (gui->input) {
		gui_handleInput(gui);
	}
	if (gui->top->v->w_tick != widget_tick) {
		guiDebugPrint("wtick");
		guiDebugPrint("%d %d" _C_ gui->top->v->w_tick _C_ container_tick);
		guiDebugPrint("%d" _C_ list_size(((guiContainer_t*)(gui->top))->children));
		gui->top->v->w_tick(gui->top);
	}
	guiDebugPrint("gui tick end");
}

void gui_setInput(guiGUI_t* gui, guiInput_t* newInput)
{
	gui->input = newInput;
}

guiInput_t* gui_getInput(const guiGUI_t* gui)
{
	return gui->input;
}

static void gui_handleInput(guiGUI_t* gui)
{
	guiDebugPrint("handle input");
	if (!gui->input || !gui->input->mouseEventQueue) {
		return;
		// FIXME error
	}
	
	while (queue_size(gui->input->mouseEventQueue) > 0) {
		guiDebugPrint("dequeue mouse input");
		event_t *event = (event_t*)queue_front(gui->input->mouseEventQueue);
		mouseEvent_t *mouseEvent = event->events.mouse;
		
		switch (mouseEvent->type) {
			case ME_PRESSED: {
				gui_handleMousePressed(gui, event);
			}
			break;
			case ME_RELEASED: {
				gui_handleMouseReleased(gui, event);
			}
			break;
		}
		queue_pop(gui->input->mouseEventQueue);
	}
	guiDebugPrint("end handle input");
}

guiWidget_t* gui_getWidgetAt(guiGUI_t* gui, int x, int y)
{
	guiWidget_t* parent = gui->top;
	guiWidget_t* child = gui->top;

	while (child != NULL) {
		guiWidget_t *swap = child;
		int parentX, parentY;
		widget_getAbsolutePosition(parent, &parentX, &parentY);
		child = parent->v->w_getWidgetAt(parent, x - parentX, y - parentY);
		parent = swap;
	}

	return parent;
}

static void gui_handleMousePressed(guiGUI_t* gui, event_t* event)
{
	mouseEvent_t *mouseEvent = event->events.mouse;
	guiWidget_t* sourceWidget = gui_getMouseEventSource(gui, mouseEvent->x, mouseEvent->y);
	gui_distributeEvent(gui, sourceWidget, event);
	
}

static void gui_handleMouseReleased(guiGUI_t* gui, event_t* event)
{
	
}

guiWidget_t* gui_getMouseEventSource(guiGUI_t* gui, int x, int y)
{
	guiWidget_t* widget = gui_getWidgetAt(gui, x, y);
	return widget;
}

bool gui_isWidgetExisting(guiGUI_t* gui, const guiWidget_t* widget)
{
	return gui->top->v->w_isWidgetExisting(gui->top, widget);
}

static void gui_distributeEvent(guiGUI_t* gui, guiWidget_t* source, event_t* event)
{
	guiWidget_t* parent = source;
	guiWidget_t* widget = source;
	
	while (parent != NULL) {
		if (!gui_isWidgetExisting(gui, widget)) {
			return;
		}
		
		parent = widget_getParent(widget);
		if (widget->flags & WF_ENABLED) {
			switch (event->type) {
				case EV_Mouse:
					gui_distributeMouseEvent(gui, widget, event->events.mouse);
					break;
				
			}
		}
		
		guiWidget_t* swap = widget;
		widget = parent;
		parent = widget_getParent(swap);
		
	}
}

static void gui_distributeMouseEvent(guiGUI_t* gui, guiWidget_t* widget, mouseEvent_t* event)
{
	int widgetX, widgetY;
	widget_getAbsolutePosition(widget, &widgetX, &widgetY);
	
	event->x -= widgetX;
	event->x -= widgetY;
	
	list_t *listeners = widget_getListeners(widget);
	for (listNode_t *node = listeners->head; node; node = node->next) {
		eventListener_t *it = (eventListener_t*)node->data;
		if (it->type == EV_Mouse) {
			mouseListener_t *mouse = it->listeners.mouseListener;
			if (mouse->type == event->type) {
				switch (mouse->type) {
					case ME_MOVED:
						mouse->types.mouseMoved(widget, event);
						break;
					case ME_PRESSED:
						mouse->types.mousePressed(widget, event);
						break;
					case ME_RELEASED:
						mouse->types.mouseReleased(widget, event);
						break;
				}
			}
		}
	}
	
}
