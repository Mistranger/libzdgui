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
	if (!gui->top || !gui->graphics) {
		guiError("No top widget or graphics object!");
		return;
	}
	
	if (!(gui->top->flags & WF_VISIBLE)) {
		return;
	}
	
	graph_beginDraw(gui->graphics);
	guiDebugPrint("drawing top widget");
	graph_pushClipArea(gui->graphics, gui->top->dim);
	gui->top->v->w_draw(gui->top, gui->graphics);
	graph_popClipArea(gui->graphics);

	if (gui->mouse) {
		mouse_drawCursor(gui->mouse, gui->graphics);
	}

	graph_endDraw(gui->graphics);
}


void gui_tick(guiGUI_t* gui)
{
	guiDebugPrint("gui tick");
	if (!gui->top) {
		// FIXME error
		return;
	}
	if (gui->mouse) {
		mouse_getInput(gui->mouse, gui->graphics);
		gui_handleMouseInput(gui);
	}
	if (gui->top->v->w_tick != widget_tick) {
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

void gui_setMouse(guiGUI_t* gui, guiMouse_t* newMouse)
{
	gui->mouse = newMouse;
}

guiMouse_t* gui_getMouse(const guiGUI_t* gui)
{
	return gui->mouse;
}

static void gui_handleMouseInput(guiGUI_t* gui)
{
	guiDebugPrint("handle mouse input");
	if (!gui->mouse->mouseEventQueue) {
		guiError("no mouse queue!");
		return;
		// FIXME error
	}
	
	while (queue_size(gui->mouse->mouseEventQueue) > 0) {
		guiInfo("dequeue mouse mouse");
		event_t *event = (event_t*)queue_front(gui->mouse->mouseEventQueue);
		if (event->type != EV_Mouse) {
			guiError("Bad event type");
			return;
		}
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
			default: {
				guiError("Unknown mouse event");
			}
			break;
		}
		queue_pop(gui->mouse->mouseEventQueue);
	}
	guiDebugPrint("end handle mouse");
}

guiWidget_t* gui_getWidgetAt(guiGUI_t* gui, vec2i_t pos)
{
	guiWidget_t* parent = gui->top;
	guiWidget_t* child = gui->top;

	while (child != NULL) {
		guiWidget_t *swap = child;
		vec2i_t parentPos;
		widget_getAbsolutePosition(parent, &parentPos);
		vec2i_t at;
		vec_sub2(at, pos, parentPos);
		child = parent->v->w_getWidgetAt(parent, at);
		parent = swap;
	}

	return parent;
}

static void gui_handleMousePressed(guiGUI_t* gui, event_t* event)
{
	mouseEvent_t *mouseEvent = event->events.mouse;
	guiWidget_t* sourceWidget = gui_getMouseEventSource(gui, mouseEvent->pos);
	gui_distributeEvent(gui, sourceWidget, event);
	
}

static void gui_handleMouseReleased(guiGUI_t* gui, event_t* event)
{
	mouseEvent_t *mouseEvent = event->events.mouse;
	guiWidget_t* sourceWidget = gui_getMouseEventSource(gui, mouseEvent->pos);
	gui_distributeEvent(gui, sourceWidget, event);
}

guiWidget_t* gui_getMouseEventSource(guiGUI_t* gui, vec2i_t pos)
{
	guiWidget_t* widget = gui_getWidgetAt(gui, pos);
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
	
	guiInfo("distributing mouse event");
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
	vec2i_t widgetPos;
	widget_getAbsolutePosition(widget, &widgetPos);
	
	vec_sub(event->pos, widgetPos);
	
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
