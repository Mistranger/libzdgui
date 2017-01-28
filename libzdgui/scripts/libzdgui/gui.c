#include "system.h"

#include "gui.h"

#include "debug.h"
#include "event.h"

#include "graphics.h"
#include "widget.h"
#include "util/util.h"
#include "widgets/container.h"

void gui_init(guiGUI *gui, int screenWidth, int screenHeight)
{	
	gui->top = NULL;
	gui->graphics = new (guiGraphics);
	graph_init(gui->graphics, screenWidth, screenHeight);
	gui->input = new (guiInput);
	input_init(gui->input);
	gui->mouse = new (guiMouse);
	mouse_init(gui->mouse);
	gui->allWidgets = list_new();
	gui->widgetsUnderMouse = list_new();
	gui->focusManager = focusManager_new();
	guiInfo("gui system initialized");
}



void gui_destroy(guiGUI *gui)
{
	if (gui_widgetExists(gui, gui->top)) {
		gui_setTop(gui, NULL);
	}
	focusManager_delete(gui->focusManager);

	// Destruction
	list_delete(gui->allWidgets);
}

void gui_setTop(guiGUI *gui, guiWidget *newTop)
{
	if (gui->top) {
		gui->top->v->w_setFocusManager(gui->top, NULL);
	}
	if (newTop) {
		newTop->v->w_setFocusManager(newTop, gui->focusManager);
	}
	gui->top = newTop;
}

void gui_handleMouseMove(guiGUI *gui, guiMouseEvent *event)
{
	listNode_t *it;
	bool done = list_size(gui->widgetsUnderMouse) == 0;
	size_t i;
	guiWidget *w;
	vec2i pos;

	while (!done) {
		i = 0;
		for (it = gui->widgetsUnderMouse->head; it; it = it->next) {
			w = (guiWidget *)it->data;
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
					guiMouseEvent *left = mouseEvent_new(w, &event->pos, event->button, ME_LEFT);
					gui_distributeEvent(gui, w, (guiEvent*)left, true, true);
					list_erase(gui->widgetsUnderMouse, it);
					break;
				}
			}
			++i;
		}
		done = (i == list_size(gui->widgetsUnderMouse));
	}

	guiWidget *source = gui_getMouseEventSource(gui, &event->pos);
	if (!source) {
		return;
	}
	guiWidget *parent = source;
	guiWidget *widget = parent;
	guiWidget *swap;
	bool found;

	if (focus_getModalMouseInputFocused(gui->focusManager) != NULL
		&& widget == focus_getModalMouseInputFocused(gui->focusManager)
		&& gui_widgetExists(gui, widget)) {
		vec2i pos;
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
				guiMouseEvent *entered = mouseEvent_new(widget, &event->pos, event->button, ME_ENTERED);
				gui_distributeEvent(gui, widget, (guiEvent*)entered, true, true);
				list_push_front(gui->widgetsUnderMouse, widget);
			}
			swap = widget;
			widget = parent;
			parent = widget_getParent(swap);
		} while (parent != NULL);

	}


	if (focus_getDragged(gui->focusManager)) {
		guiMouseEvent *dragged = mouseEvent_new(focus_getDragged(gui->focusManager),
											   &event->pos, mouse_getLastDragButton(gui->mouse), ME_DRAGGED);
		gui_distributeEvent(gui, focus_getDragged(gui->focusManager), (guiEvent*)dragged, false, false);
	} else {
		widget = source;
		guiMouseEvent *moved = mouseEvent_new(widget, &event->pos, event->button, ME_MOVED);
		gui_distributeEvent(gui, widget, (guiEvent*)moved, false, false);
	}

}

void gui_handleMouse(guiGUI *gui, guiMouseEvent *event)
{
	if (event->type == ME_MOVED) {
		gui_handleMouseMove(gui, event);
	} else {
		guiWidget *sourceWidget = gui_getMouseEventSource(gui, &event->pos);
		if (focus_getDragged(gui->focusManager) != NULL) {
			if (event->type == ME_RELEASED) {
				if (sourceWidget != focus_getLastPressed(gui->focusManager)) {
					focus_setLastPressed(gui->focusManager, NULL);
				}
			}
			sourceWidget = focus_getDragged(gui->focusManager);
		}

		if (sourceWidget) {
			event_setSource(event, sourceWidget);
			gui_distributeEvent(gui, sourceWidget, (guiEvent *)event, false, false);
			if (event->type == ME_RELEASED) {
				if (event->button == mouse_getLastPressButton(gui->mouse)
					&& focus_getLastPressed(gui->focusManager) == sourceWidget) {
					guiMouseEvent *clicked = mouseEvent_new(sourceWidget, &event->pos, event->button, ME_CLICKED);
					gui_distributeEvent(gui, sourceWidget, (guiEvent *)clicked, false, false);
					focus_setLastPressed(gui->focusManager, NULL);
				} else {
					mouse_setLastPressButton(gui->mouse, MB_EMPTY);
				}
				if (focus_getDragged(gui->focusManager) != NULL) {
					focus_setDragged(gui->focusManager, NULL);
				}
			} else if (event->type == ME_PRESSED) {
				focus_setLastPressed(gui->focusManager, sourceWidget);
				if (focus_getModalFocused(gui->focusManager) != NULL
					&& widget_hasModalFocus(sourceWidget)
					|| focus_getModalFocused(gui->focusManager) == NULL) {
					widget_requestFocus(sourceWidget);
				}

				focus_setDragged(gui->focusManager, sourceWidget);
				mouse_setLastDragButton(gui->mouse, event->button);
				mouse_setLastPressButton(gui->mouse, event->button);
			}

		}
	}
}

void gui_draw(guiGUI *gui)
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

void gui_handleModalMouseInputFocus(guiGUI *gui)
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

void gui_handleModalFocus(guiGUI *gui)
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

void gui_handleModalFocusGained(guiGUI *gui)
{
	while (list_size(gui->widgetsUnderMouse) > 0) {
		guiWidget *w = (guiWidget *)list_front(gui->widgetsUnderMouse)->data;

		if (gui_widgetExists(gui, w)) {
			guiMouseEvent *exited = mouseEvent_new(w, &gui->mouse->lastMousePos,
												  mouse_getLastPressButton(gui->mouse), ME_LEFT);
			gui_distributeEvent(gui, w, (guiEvent *)exited, true, true);
		}
		list_pop_front(gui->widgetsUnderMouse);
	}
	focus_setLastWithModalMouseInputFocus(gui->focusManager, focus_getLastWithModalMouseInputFocus(gui->focusManager));
}

void gui_handleModalFocusReleased(guiGUI *gui)
{
	guiWidget *widget = gui_getMouseEventSource(gui, &gui->mouse->lastMousePos);
	if (!widget) {
		return;
	}
	guiWidget *parent = widget;
	guiWidget *swap;

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
			guiMouseEvent *entered = mouseEvent_new(widget, &gui->mouse->lastMousePos,
												   mouse_getLastPressButton(gui->mouse), ME_ENTERED);
			gui_distributeEvent(gui, widget, (guiEvent*)entered, false, true);
			list_push_front(gui->widgetsUnderMouse, widget);
		}
		swap = widget;
		widget = parent;
		parent = widget_getParent(swap);
	}
}

void gui_tick(guiGUI *gui)
{
	if (!gui->top) {
		guiError("No top widget set");
		return;
	}
	gui_handleModalFocus(gui);
	gui_handleModalMouseInputFocus(gui);

	if (gui->mouse) {
		mouse_getInput(gui->mouse, gui->graphics);
		gui_handleMouseInput(gui);
	}
	if (gui->top->v->w_tick != widget_tick) {
		gui->top->v->w_tick(gui->top);
	}
}

void gui_handleMouseInput(guiGUI *gui)
{
	if (!gui->mouse->mouseEventQueue) {
		guiError("no mouse queue!");
		return;
	}

	guiEvent *event;
	while (queue_size(gui->mouse->mouseEventQueue) > 0) {
		event = (guiEvent *)queue_front(gui->mouse->mouseEventQueue);

		if (event->eventType != EV_Mouse) {
			guiError("Bad event type");
			return;
		}

		gui->mouse->lastMousePos = ((guiMouseEvent *)event)->pos;

		gui_handleMouse(gui, (guiMouseEvent *)event);

		queue_pop(gui->mouse->mouseEventQueue);
	}
}

guiWidget *gui_getWidgetAt(guiGUI *gui, vec2i *pos)
{
	if (!rect_isPointInRect(&widget_getDimensions(gui->top), *pos)) {
		return NULL;
	}

	guiWidget *parent = gui->top;
	guiWidget *child = gui->top;
	vec2i parentPos;
	guiWidget *swap;
	vec2i at;

	while (child != NULL) {
		swap = child;
		widget_getAbsolutePosition(parent, &parentPos);
		vec_sub2(at, *pos, parentPos);
		child = parent->v->w_getWidgetAt(parent, at);
		parent = swap;
	}

	return parent;
}

guiWidget *gui_getMouseEventSource(guiGUI *gui, vec2i *pos)
{
	guiWidget *w = gui_getWidgetAt(gui, pos);
	if (w && focus_getModalMouseInputFocused(gui->focusManager) != NULL
		&& !widget_hasModalMouseInputFocus(w)) {
		return focus_getModalMouseInputFocused(gui->focusManager);
	}
	return w;
}

void gui_addWidget(guiGUI *gui, void *widget)
{
	list_push_back(gui->allWidgets, widget);
	((guiWidget*)widget)->gui = gui;
}

bool gui_widgetExists(guiGUI *gui, guiWidget *widget)
{
	if (!widget) {
		return false;
	}
	for (listNode_t *it = gui->allWidgets->head; it; it = it->next) {
		if (it->data == widget) {
			return true;
		}
	}
	return false;
}

void gui_deleteWidget(guiGUI *gui, guiWidget *widget)
{
	list_remove(gui->allWidgets, widget);
	widget->gui = NULL;
}

void gui_distributeEvent(guiGUI *gui, guiWidget *source, guiEvent *event, bool force, bool toSourceOnly)
{
	guiWidget *parent = source;
	guiWidget *widget = source;
	guiWidget *swap;

	if (focus_getModalFocused(gui->focusManager) != NULL
		&& !widget_hasModalFocus(widget) && !force) {
		if (event) {
			free(event);
		}
		return;
	}

	if (focus_getModalMouseInputFocused(gui->focusManager) != NULL
		&& !widget_hasModalMouseInputFocus(widget) && !force)	{
		if (event) {
			free(event);
		}
		return;
	}

	while (parent != NULL) {
		if (!gui_widgetExists(gui, widget)) {
			if (event) {
				free(event);
			}
			return;
		}
		
		parent = widget_getParent(widget);
		if (widget_isEnabled(widget) || force) {
			switch (event->eventType) {
				case EV_Mouse: {
					guiMouseEvent *mouseEvent = (guiMouseEvent*)event;
					vec2i widgetPos;
					widget_getAbsolutePosition(widget, &widgetPos);
					vec_sub(mouseEvent->pos, widgetPos);
					widget_handleEvent(widget, event, false);
					
				}
				break;
				default:
				widget_handleEvent(widget, event, false);
				break;
			}
		}
		
		if (toSourceOnly) {
			break;
		}

		swap = widget;
		widget = parent;
		parent = widget_getParent(swap);

		if (focus_getModalFocused(gui->focusManager) != NULL
			&& !widget_hasModalFocus(widget)) {
			if (event) {
				free(event);
			}
			return;
		}

		if (focus_getModalMouseInputFocused(gui->focusManager) != NULL
			&& !widget_hasModalMouseInputFocus(widget)) {
			if (event) {
				free(event);
			}
			return;
		}
	}
	if (event) {
		free(event);
	}
}

