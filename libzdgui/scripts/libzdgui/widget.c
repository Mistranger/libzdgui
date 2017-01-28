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

const char *widget_typename(struct guiWidget_s *widget)
{
	return WidgetType;
}

void widget_destructor(guiWidget_t *widget)
{
	lifecycleEvent_t *destroyed = lifecycleEvent_new(widget, LE_DESTROYED);
	widget_handleEvent(widget, (event_t *)destroyed, true);

	// Destruction
	list_delete(widget->eventListeners);
	free(widget);
}

void widget_addListener(void *widget, void *listener)
{
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void *)listener);
}

mouseListener_t *widget_addMouseListener(void *widget,
										 mouseEventType_t mouseType, void(*listenerFunc)(void *widget, mouseEvent_t *event))
{
	mouseListener_t *listener = mouseListener_new(widget, mouseType, listenerFunc);

	list_push_back(((guiWidget_t *)widget)->eventListeners, (void *)listener);
	return listener;
}

dimensionListener_t *widget_addDimensionListener(void *widget,
												 dimensionEventType_t dimType, void(*listenerFunc)(void *widget, dimensionEvent_t *event))
{
	dimensionListener_t *listener = dimensionListener_new(widget, dimType, listenerFunc);
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void *)listener);
	return listener;
}

lifecycleListener_t *widget_addLifeCycleListener(void *widget,
												 lifecycleEventType_t lifeType, void(*listenerFunc)(void *widget, lifecycleEvent_t *event))
{
	lifecycleListener_t *listener = lifecycleListener_new(widget, lifeType, listenerFunc);
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void *)listener);
	return listener;
}

void widget_removeListener(void *widget, void *listener)
{
	list_remove(((guiWidget_t *)widget)->eventListeners, listener);
}

void widget_tick(guiWidget_t *widget)
{
	return;
}

void widget_draw(const guiWidget_t *widget, guiGraphics_t *graphics)
{
	return;
}

void widget_init(guiWidget_t *widget)
{
	guiRectangle_t dim = {0, 0, 0, 0};
	widget->dim = dim;

	widget->isVisible = true;
	widget->isEnabled = true;
	widget->isFocusable = true;
	widget->isContainer = false;

	widget->eventListeners = list_new();
	widget->font = (guiFont_t *)&FONT_CONFONT;
	widget->parent = NULL;
	widget->v = &guiWidget_vtable;

	guiInfo("new widget created");
}

guiRectangle_t *widget_getChildrenArea(const guiWidget_t *widget)
{
	return &(guiRectangle_t) {0, 0, 0, 0};
}

guiWidget_t *widget_getWidgetAt(const guiWidget_t *widget, vec2i_t pos)
{
	return NULL;
}

bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist)
{
	return widget == exist ? true : false;
}

void widget_setFocusManager(guiWidget_t *widget, void *focus)
{
	if (widget->focusManager) {
		widget_releaseModalFocus(widget);
		focus_removeFromManager(widget->focusManager, widget);
	}

	if (focus) {
		focus_addToManager((guiFocusManager_t *)focus, widget);
	}

	widget->focusManager = focus;
}

void _widget_setFocusable(guiWidget_t *widget, bool state)
{
	if (!state && ((guiWidget_t *)widget)->isFocusable) {
		focus_focusNone(((guiWidget_t *)widget)->focusManager);
	}
	((guiWidget_t *)widget)->isFocusable = state;
}

void widget_requestFocus(guiWidget_t *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return;
	}

	if (widget_isFocusable(widget))	{
		focus_requestFocus(widget->focusManager, widget);
	}
}

void widget_requestModalFocus(guiWidget_t *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return;
	}
	focus_requestModalFocus(widget->focusManager, widget);
}

void widget_requestModalMouseInputFocus(guiWidget_t *widget)
{
	if (!widget->focusManager) {
		guiError("No focushandler set (did you add the widget to the gui?).");
		return;
	}
	focus_requestModalMouseInputFocus(widget->focusManager, widget);
}

void widget_releaseModalFocus(guiWidget_t *widget)
{
	if (!widget->focusManager) {
		return;
	}
	focus_releaseModalFocus(widget->focusManager, widget);
}

void widget_releaseModalMouseInputFocus(guiWidget_t *widget)
{
	if (!widget->focusManager) {
		return;
	}
	focus_releaseModalMouseInputFocus(widget->focusManager, widget);
}

bool widget_hasModalFocus(const guiWidget_t *widget)
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

bool widget_hasModalMouseInputFocus(const guiWidget_t *widget)
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

void widget_setDimension(guiWidget_t *widget, const guiRectangle_t newDim)
{
	guiRectangle_t oldDim = widget->dim;
	widget->dim = newDim;
	if (widget->dim.width != oldDim.width || widget->dim.height != oldDim.height) {
		dimensionEvent_t *event = dimensionEvent_new((void *)widget, &newDim.pos, newDim.width, newDim.height, DE_RESIZED);
		widget_handleEvent(widget, (event_t *)event, true);
	}
	if (widget->dim.pos.x != oldDim.pos.x || widget->dim.pos.y != oldDim.pos.y) {
		dimensionEvent_t *event = dimensionEvent_new(widget, &newDim.pos, newDim.width, newDim.height, DE_MOVED);
		widget_handleEvent(widget, (event_t *)event, true);
	}
}

void widget_getAbsolutePosition(guiWidget_t *widget, vec2i_t *pos)
{
	if (!widget->parent) {
		pos->x = widget->dim.pos.x;
		pos->y = widget->dim.pos.y;
		return;
	}

	vec2i_t parentPos;

	widget_getAbsolutePosition(widget->parent, &parentPos);

	guiRectangle_t rect = *widget->parent->v->w_getChildrenArea(widget->parent);
	pos->x = parentPos.x + widget->dim.pos.x + rect.pos.x;
	pos->y = parentPos.y + widget->dim.pos.y + rect.pos.y;
}

void widget_handleEvent(guiWidget_t *widget, event_t *event, bool freeEvent)
{
	if (!widget || !event) {
		guiError("No widget handler or event to handle!");
		return;
	}
	eventListener_t *listener;

	for (listNode_t *node = widget->eventListeners->head; node; node = node->next) {
		listener = (eventListener_t *)node->data;
		if (listener->listenerType == event->eventType) {
			listener->handleEvent(listener, event);
		}
	}
	if (freeEvent) {
		free(event);
	}
}
