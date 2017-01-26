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
	gui->allWidgets = list_new();
	gui->widgetsUnderMouse = list_new();
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

void gui_handleMouseMove(guiGUI_t* gui, mouseEvent_t* event)
{
	listNode_t *it;
	listNode_t *remove;
	bool done = list_size(gui->widgetsUnderMouse) == 0;
	size_t i;
	guiWidget_t *w;
	vec2i_t pos;
	
	while (!done) {
		i = 0;
		for (it = gui->widgetsUnderMouse->head; it; it = it->next) {
			w = (guiWidget_t*)it->data;
			if (!w || !gui_widgetExists(gui, w)) {
				list_erase(gui->widgetsUnderMouse, it);
				
				break;
			} else {
				widget_getAbsolutePosition(w, &pos);
				if (pos.x > event->pos.x
					|| pos.y > event->pos.y
					|| pos.x + widget_getWidth(w) <= event->pos.x
					|| pos.y + widget_getHeight(w) <= event->pos.y
					|| !widget_isVisible(w)) {
						
						mouseEvent_t *left = new(mouseEvent_t);
						*left = *event;
						left->type = ME_LEFT;
						gui_distributeMouseEvent(gui, w, left);
						list_erase(gui->widgetsUnderMouse, it);
						
						break;
				}
			}
			++i;
		}
		done = (i == list_size(gui->widgetsUnderMouse));
	}
	guiWidget_t* source = gui_getMouseEventSource(gui, &event->pos);
	guiWidget_t* parent = source;
	guiWidget_t* widget = parent;
	guiWidget_t *swap;
	bool found;

	if (parent != NULL) {
		do {
			parent = widget_getParent(widget);

			found = false;
			for (it = gui->widgetsUnderMouse->head; it; it = it->next) {
				if (it->data == widget) {
					found = true;
					break;
				}
			}
			if (!found && gui_widgetExists(gui, widget)) {
				guiInfo("Mouse entered widget");
				
				mouseEvent_t *entered = new(mouseEvent_t);
				*entered = *event;
				entered->type = ME_ENTERED;
				gui_distributeMouseEvent(gui, widget, entered);
				list_push_front(gui->widgetsUnderMouse, widget);
			} 
			swap = widget;
			widget = parent;
			parent = widget_getParent(swap);
		} while (parent != NULL);
		
		widget = source;
		mouseEvent_t *moved = new(mouseEvent_t);
		*moved = *event;
		moved->type = ME_MOVED;
		gui_distributeMouseEvent(gui, widget, moved);
	}
}

void gui_handleMouse(guiGUI_t* gui, mouseEvent_t* event)
{
	if (event->type == ME_MOVED) {
		gui_handleMouseMove(gui, event);
	} else {
		guiWidget_t* sourceWidget = gui_getMouseEventSource(gui, &event->pos);
		if (sourceWidget) {
			gui_distributeEvent(gui, sourceWidget, (event_t*)event);
		}
	}	
}

void gui_draw(guiGUI_t* gui)
{
	if (!gui->top || !gui->graphics) {
		guiError("No top widget or graphics object!");
		return;
	}
	
	if (!(widget_isVisible(gui->top))) {
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

void gui_handleMouseInput(guiGUI_t* gui)
{
	if (!gui->mouse->mouseEventQueue) {
		guiError("no mouse queue!");
		return;
	}
	
	event_t *event;
	while (queue_size(gui->mouse->mouseEventQueue) > 0) {
		event = (event_t*)queue_front(gui->mouse->mouseEventQueue);
		if (event->eventType != EV_Mouse) {
			guiError("Bad event type");
			return;
		}

		gui_handleMouse(gui,  (mouseEvent_t*)event);
		
		queue_pop(gui->mouse->mouseEventQueue);
	}
}

guiWidget_t* gui_getWidgetAt(guiGUI_t* gui, vec2i_t *pos)
{
	if (!rect_isPointInRect(&widget_getDimensions(gui->top), *pos)) {
		return NULL;
	}
	
	guiWidget_t* parent = gui->top;
	guiWidget_t* child = gui->top;
	vec2i_t parentPos;
	guiWidget_t *swap;
	vec2i_t at;

	while (child != NULL) {
		swap = child;
		widget_getAbsolutePosition(parent, &parentPos);
		vec_sub2(at, *pos, parentPos);
		child = parent->v->w_getWidgetAt(parent, at);
		parent = swap;
	}
	
	return parent;
}

guiWidget_t* gui_getMouseEventSource(guiGUI_t* gui, vec2i_t *pos)
{
	return gui_getWidgetAt(gui, pos);
}

void gui_addWidget(guiGUI_t *gui, void* widget)
{
	list_push_back(gui->allWidgets, widget);
}

bool gui_widgetExists(guiGUI_t *gui, guiWidget_t* widget)
{
	for (listNode_t *it = gui->allWidgets->head; it; it = it->next) {
		if (it->data == widget) {
			return true;
		}
	}
	return false;
}

void gui_deleteWidget(guiGUI_t *gui, guiWidget_t* widget)
{
	list_remove(gui->allWidgets, widget);
}

void gui_distributeEvent(guiGUI_t* gui, guiWidget_t* source, event_t* event)
{
	guiWidget_t* parent = source;
	guiWidget_t* widget = source;
	guiWidget_t* swap;

	while (parent != NULL) {
		if (!gui_widgetExists(gui, widget)) {
			return;
		}
		
		parent = widget_getParent(widget);
		if (widget_isEnabled(widget)) {
			switch (event->eventType) {
				case EV_Mouse:
					gui_distributeMouseEvent(gui, widget, (mouseEvent_t*)event);
					break;
				
			}
		}
		
		swap = widget;
		widget = parent;
		parent = widget_getParent(swap);
		
	}
}

void gui_distributeMouseEvent(guiGUI_t* gui, guiWidget_t* widget, mouseEvent_t* event)
{
	vec2i_t widgetPos;
	widget_getAbsolutePosition(widget, &widgetPos);
	
	vec_sub(event->pos, widgetPos);
	
	list_t *listeners = widget_getListeners(widget);
	eventListener_t *it;
	
	for (listNode_t *node = listeners->head; node; node = node->next) {
		it = (eventListener_t*)node->data;
		if (it->listenerType == EV_Mouse) {
			mouseListener_t *mouse = (mouseListener_t*)it;
			if (mouse->type == event->type) {
				switch (mouse->type) {
					case ME_MOVED:
						mouse->types.mouseMoved(mouse->listener.handlerWidget, event);
						break;
					case ME_PRESSED:
						mouse->types.mousePressed(mouse->listener.handlerWidget, event);
						break;
					case ME_RELEASED:
						mouse->types.mouseReleased(mouse->listener.handlerWidget, event);
						break;
					case ME_ENTERED:
						mouse->types.mouseEntered(mouse->listener.handlerWidget, event);
						break;
					case ME_LEFT:
						mouse->types.mouseLeft(mouse->listener.handlerWidget, event);
						break;
					case ME_CLICKED:
						mouse->types.mouseClicked(mouse->listener.handlerWidget, event);
						break;
					default:
						guiError("Unknown mouse event type");
						break;

				}
			}
		}
	}
	
}
