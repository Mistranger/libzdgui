#include "system.h"

#include "event/event_dimension.h"
#include "event/event_lifecycle.h"
#include "event/event_mouse.h"
#include "focusmanager.h"
#include "widget.h"

const char *WidgetType = "Widget";

guiWidget_vf_t guiWidget_vtable = {
	widget_typename,
	widget_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	widget_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager
};

const char *widget_typename(struct guiWidget *widget)
{
	return WidgetType;
}

void widget_destructor(guiWidget *widget)
{
	guiLifecycleEvent *destroyed = lifecycleEvent_new(widget, LE_DESTROYED);
	widget_handleEvent(widget, (guiEvent *)destroyed, true);

	// Destruction
	list_delete(widget->eventListeners);
	free(widget);
}

void widget_addListener(void *widget, void *listener)
{
	list_push_back(((guiWidget *)widget)->eventListeners, (void *)listener);
}

guiMouseListener *widget_addMouseListener(void *widget,
										 mouseEventType_t mouseType, void(*listenerFunc)(void *widget, guiMouseEvent *event))
{
	guiMouseListener *listener = mouseListener_new(widget, mouseType, listenerFunc);

	list_push_back(((guiWidget *)widget)->eventListeners, (void *)listener);
	return listener;
}

dimensionListener_t *widget_addDimensionListener(void *widget,
												 guiDimensionEventType dimType, void(*listenerFunc)(void *widget, guiDimensionEvent *event))
{
	dimensionListener_t *listener = dimensionListener_new(widget, dimType, listenerFunc);
	list_push_back(((guiWidget *)widget)->eventListeners, (void *)listener);
	return listener;
}

guiLifecycleListener *widget_addLifeCycleListener(void *widget,
												 guiLifecycleEventType lifeType, void(*listenerFunc)(void *widget, guiLifecycleEvent *event))
{
	guiLifecycleListener *listener = lifecycleListener_new(widget, lifeType, listenerFunc);
	list_push_back(((guiWidget *)widget)->eventListeners, (void *)listener);
	return listener;
}

void widget_removeListener(void *widget, void *listener)
{
	list_remove(((guiWidget *)widget)->eventListeners, listener);
}

void widget_tick(guiWidget *widget)
{
	return;
}

void widget_draw(const guiWidget *widget, guiGraphics *graphics)
{
	return;
}

void widget_init(guiWidget *widget)
{
	guiRectangle dim = {0, 0, 0, 0};
	widget->dim = dim;

	widget->isVisible = true;
	widget->isEnabled = true;
	widget->isFocusable = true;
	widget->isContainer = false;

	widget->eventListeners = list_new();
	widget->font = (guiFont *)&FONT_CONFONT;
	widget->parent = NULL;
	widget->gui = NULL;
	widget->v = &guiWidget_vtable;

	guiInfo("new widget created");
}

guiRectangle *widget_getChildrenArea(const guiWidget *widget)
{
	return &(guiRectangle) {0, 0, 0, 0};
}

guiWidget *widget_getWidgetAt(const guiWidget *widget, vec2i pos)
{
	return NULL;
}

bool widget_isWidgetExisting(guiWidget *widget, const guiWidget *exist)
{
	return widget == exist ? true : false;
}

void widget_setFocusManager(guiWidget *widget, void *focus)
{
	if (widget->focusManager) {
		widget_releaseModalFocus(widget);
		focus_removeFromManager(widget->focusManager, widget);
	}

	if (focus) {
		focus_addToManager((guiFocusManager *)focus, widget);
	}

	widget->focusManager = focus;
}

void _widget_setFocusable(guiWidget *widget, bool state)
{
	if (!state && widget_isFocused(widget)) {
		focus_focusNone(((guiWidget *)widget)->focusManager);
	}
	((guiWidget *)widget)->isFocusable = state;
}

void widget_requestFocus(guiWidget *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return;
	}

	if (widget_isFocusable(widget))	{
		focus_requestFocus(widget->focusManager, widget);
	}
}

void widget_requestModalFocus(guiWidget *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return;
	}
	focus_requestModalFocus(widget->focusManager, widget);
}

void widget_requestModalMouseInputFocus(guiWidget *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return;
	}
	focus_requestModalMouseInputFocus(widget->focusManager, widget);
}

void widget_releaseModalFocus(guiWidget *widget)
{
	if (!widget->focusManager) {
		return;
	}
	focus_releaseModalFocus(widget->focusManager, widget);
}

void widget_releaseModalMouseInputFocus(guiWidget *widget)
{
	if (!widget->focusManager) {
		return;
	}
	focus_releaseModalMouseInputFocus(widget->focusManager, widget);
}

bool widget_hasModalFocus(const guiWidget *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return false;
	}
	if (widget_getParent(widget)) {
		return (focus_getModalFocused(widget->focusManager) == widget) || widget_hasModalFocus(widget_getParent(widget));
	}
	return focus_getModalFocused(widget->focusManager) == widget;
}

bool widget_hasModalMouseInputFocus(const guiWidget *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return false;
	}
	if (widget_getParent(widget)) {
		return (focus_getModalMouseInputFocused(widget->focusManager) == widget)
			   || widget_hasModalMouseInputFocus(widget_getParent(widget));
	}
	return focus_getModalFocused(widget->focusManager) == widget;
}

void widget_setDimension(guiWidget *widget, const guiRectangle newDim)
{
	guiRectangle oldDim = widget->dim;
	widget->dim = newDim;
	if (widget->dim.width != oldDim.width || widget->dim.height != oldDim.height) {
		guiDimensionEvent *event = dimensionEvent_new((void *)widget, &newDim.pos, newDim.width, newDim.height, DE_RESIZED);
		widget_handleEvent(widget, (guiEvent *)event, true);
	}
	if (widget->dim.pos.x != oldDim.pos.x || widget->dim.pos.y != oldDim.pos.y) {
		guiDimensionEvent *event = dimensionEvent_new(widget, &newDim.pos, newDim.width, newDim.height, DE_MOVED);
		widget_handleEvent(widget, (guiEvent *)event, true);
	}
}

void widget_getAbsolutePosition(guiWidget *widget, vec2i *pos)
{
	if (!widget->parent) {
		pos->x = widget->dim.pos.x;
		pos->y = widget->dim.pos.y;
		return;
	}

	vec2i parentPos;

	widget_getAbsolutePosition(widget->parent, &parentPos);

	guiRectangle rect = *widget->parent->v->w_getChildrenArea(widget->parent);
	pos->x = parentPos.x + widget->dim.pos.x + rect.pos.x;
	pos->y = parentPos.y + widget->dim.pos.y + rect.pos.y;
}

void widget_handleEvent(guiWidget *widget, guiEvent *event, bool freeEvent)
{
	if (!widget || !event) {
		guiError("No widget handler or event to handle!");
		return;
	}
	guiEventListener *listener;

	for (listNode_t *node = widget->eventListeners->head; node; node = node->next) {
		listener = (guiEventListener *)node->data;
		if (listener->listenerType == event->eventType) {
			listener->handleEvent(listener, event);
		}
	}
	if (freeEvent) {
		free(event);
	}
}
