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
	gui->focusManager = focusManager_new();
}

void gui_destroy(guiGUI_t *gui)
{
	if (gui_widgetExists(gui, gui->top)) {
		gui_setTop(gui, NULL);
	}
	focusManager_delete(gui->focusManager);

	// Destruction
	list_delete(gui->allWidgets);
}

guiWidget_t* gui_getTop(guiGUI_t* gui)
{
	return gui->top;
}

void gui_setTop(guiGUI_t* gui, guiWidget_t* newTop)
{/*
	if (gui->top) {
		widget_setFocusManager(gui->top, NULL);
	}
	if (newTop) {
		widget_setFocusManager(newTop, gui->focusManager);
	}*/
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
						mouseEvent_t *left = mouseEvent_new(w, &event->pos, event->button, ME_LEFT);
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

	if (focus_getModalMouseInputFocused(gui->focusManager) != NULL
		&& widget == focus_getModalMouseInputFocused(gui->focusManager)
		&& gui_widgetExists(gui, widget)) {
			vec2i_t pos;
			widget_getAbsolutePosition(widget, &pos);

			if (pos.x > event->pos.x
				|| pos.y > event->pos.y
				|| pos.x + widget_getWidth(widget) <= event->pos.x
				|| pos.y + widget_getHeight(widget) <= event->pos.y) {
					parent = NULL;
			}
	}

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
				
				mouseEvent_t *entered = mouseEvent_new(widget, &event->pos, event->button, ME_ENTERED);
				gui_distributeMouseEvent(gui, widget, entered);
				list_push_front(gui->widgetsUnderMouse, widget);
			} 
			swap = widget;
			widget = parent;
			parent = widget_getParent(swap);
		} while (parent != NULL);
		
	}

	if (focus_getDragged(gui->focusManager)) {
		mouseEvent_t *moved = mouseEvent_new(focus_getDragged(gui->focusManager),
			&event->pos, gui->lastMouseDragButton, ME_DRAGGED);
		gui_distributeMouseEvent(gui, widget, moved);
	} else {
		widget = source;
		mouseEvent_t *moved = mouseEvent_new(widget, &event->pos, event->button, ME_MOVED);
		gui_distributeMouseEvent(gui, widget, moved);
	}
	
}

void gui_handleMouse(guiGUI_t* gui, mouseEvent_t* event)
{
	if (event->type == ME_MOVED) {
		//gui_handleMouseMove(gui, event);
	} else {
		guiWidget_t* sourceWidget = gui_getMouseEventSource(gui, &event->pos);
		if (focus_getDragged(gui->focusManager) != NULL) {
			sourceWidget = focus_getDragged(gui->focusManager);
		}

		if (sourceWidget) {
			gui_distributeEvent(gui, sourceWidget, (event_t*)event);
		}
		focus_setLastPressed(gui->focusManager, sourceWidget);
		if (focus_getModalFocused(gui->focusManager) != NULL
			&& widget_hasModalFocus(sourceWidget)
			|| focus_getModalFocused(gui->focusManager) == NULL) {
				widget_requestFocus(sourceWidget);
		}
		focus_setDragged(gui->focusManager, sourceWidget);
		gui->lastMouseDragButton = event->button;

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
	graph_endDraw(gui->graphics);
	/*
	guiDebugPrint("drawing top widget");
	//graph_pushClipArea(gui->graphics, gui->top->dim);
	gui->top->v->w_draw(gui->top, gui->graphics);
	//graph_popClipArea(gui->graphics);

	/*if (gui->mouse) {
		mouse_drawCursor(gui->mouse, gui->graphics);
	}*/
/*
	graph_endDraw(gui->graphics);*/
}

void gui_handleModalMouseInputFocus(guiGUI_t* gui)
{
	// Check if modal mouse input focus has been gained by a widget.
	if ((focus_getLastWithModalMouseInputFocus(gui->focusManager)
		!= focus_getModalMouseInputFocused(gui->focusManager))
		&& (focus_getLastWithModalMouseInputFocus(gui->focusManager)) == NULL) {
			gui_handleModalFocusGained(gui);
			focus_setLastWithModalMouseInputFocus(gui->focusManager, focus_getModalMouseInputFocused(gui->focusManager));
	} else if ((focus_getLastWithModalMouseInputFocus(gui->focusManager)
		!= focus_getModalMouseInputFocused(gui->focusManager))
		&& (focus_getLastWithModalMouseInputFocus(gui->focusManager) != NULL)) {
			gui_handleModalFocusReleased(gui);
			focus_setLastWithModalMouseInputFocus(gui->focusManager, NULL);
	}
}

void gui_handleModalFocus(guiGUI_t* gui)
{
	// Check if modal focus has been gained by a widget.
	if ((focus_getLastWithModalFocus(gui->focusManager)
		!= focus_getModalFocused(gui->focusManager))
		&& (focus_getLastWithModalFocus(gui->focusManager)) == NULL) {
			gui_handleModalFocusGained(gui);
			focus_setLastWithModalFocus(gui->focusManager, focus_getModalFocused(gui->focusManager));
	} else if ((focus_getLastWithModalFocus(gui->focusManager)
		!= focus_getModalFocused(gui->focusManager))
		&& (focus_getLastWithModalFocus(gui->focusManager)) == NULL) {
			gui_handleModalFocusReleased(gui);
			focus_setLastWithModalFocus(gui->focusManager, NULL);
	}
}

void gui_handleModalFocusGained(guiGUI_t* gui)
{
	while (list_size(gui->widgetsUnderMouse) > 0) {
		guiWidget_t *w = (guiWidget_t*)list_front(gui->widgetsUnderMouse)->data;

		if (gui_widgetExists(gui, w)) {
			mouseEvent_t *exited = mouseEvent_new(w, &gui->mouse->oldMouseInput->pos, 
				gui->mouse->oldMouseInput->button, ME_LEFT);
			widget_handleEvent(w, (event_t*)exited);
		}
		list_pop_front(gui->widgetsUnderMouse);
	}
	focus_setLastWithModalMouseInputFocus(gui->focusManager, focus_getLastWithModalMouseInputFocus(gui->focusManager));
}

void gui_handleModalFocusReleased(guiGUI_t* gui)
{
	guiWidget_t *widget = gui_getMouseEventSource(gui, &gui->mouse->oldMouseInput->pos);
	guiWidget_t *parent = widget;
	guiWidget_t *swap;

	bool found = false;
	while (parent != NULL) {
		parent = widget_getParent(widget);
		found = false;
		for (listNode_t *it = gui->widgetsUnderMouse->head; it; it = it->next) {
			if (it->data == widget) {
				found = true;
				break;
			}
		}
		if (!found && gui_widgetExists(gui, widget)) {
			guiInfo("Mouse entered widget");

			mouseEvent_t *entered = mouseEvent_new(widget, &gui->mouse->oldMouseInput->pos,
				gui->mouse->oldMouseInput->button, ME_ENTERED);
			gui_distributeMouseEvent(gui, widget, entered);
			list_push_front(gui->widgetsUnderMouse, widget);
		}
		swap = widget;
		widget = parent;
		parent = widget_getParent(swap);
	}
}

void gui_tick(guiGUI_t* gui)
{
	guiDebugPrint("gui tick");
	if (!gui->top) {
		guiError("No top widget set");
		return;
	}
	/*gui_handleModalFocus(gui);
	gui_handleModalMouseInputFocus(gui);

	if (gui->mouse) {
		mouse_getInput(gui->mouse, gui->graphics);
		gui_handleMouseInput(gui);
	}
	if (gui->top->v->w_tick != widget_tick) {
		gui->top->v->w_tick(gui->top);
	}
	*/
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
	guiWidget_t *w = gui_getWidgetAt(gui, pos);
	if (focus_getModalMouseInputFocused(gui->focusManager) != NULL
		&& !widget_hasModalMouseInputFocus(w)) {
		return focus_getModalMouseInputFocused(gui->focusManager);
	}
	return w;
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

	if (focus_getModalFocused(gui->focusManager) != NULL
		&& !widget_hasModalFocus(widget)) {
		return;
	}

	if (focus_getModalMouseInputFocused(gui->focusManager) != NULL
		&& !widget_hasModalMouseInputFocus(widget))	{
		return;
	}

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

		if (focus_getModalFocused(gui->focusManager) != NULL
			&& !widget_hasModalFocus(widget)) {
			return;
		}

		if (focus_getModalMouseInputFocused(gui->focusManager) != NULL
			&& !widget_hasModalMouseInputFocus(widget)) {
			return;
		}
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
			mouse->listener.handleEvent((eventListener_t*)mouse, (event_t*)event);
		}
	}
	
}
