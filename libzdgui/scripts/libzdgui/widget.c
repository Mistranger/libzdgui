#include "system.h"

#include "event/event_dimension.h"
#include "event/event_lifecycle.h"
#include "event/event_mouse.h"
#include "widget.h"

guiWidget_vf_t guiWidget_vtable = {
	widget_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	widget_draw,
	widget_tick,
	widget_isWidgetExisting
};

void widget_destructor(guiWidget_t *widget)
{
	lifecycleEvent_t *destroyed = new(lifecycleEvent_t);
	((event_t*)destroyed)->eventType = EV_LifeCycle;
	widget_handleEvent(widget, (event_t*)destroyed);

	// Destruction
	list_delete(widget->eventListeners);
	free(widget);
}

void widget_addListener(void *widget, eventListener_t *listener)
{
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void*)listener);
}

mouseListener_t* widget_addMouseListener(void *widget,
	mouseEventType_t mouseType, void(*listenerFunc)(void *widget, mouseEvent_t *event))
{
	mouseListener_t *listener = mouseListener_new(widget);
	((eventListener_t*)listener)->listenerType = EV_Mouse;
	listener->type = mouseType;
	listener->types.listen = listenerFunc;
	
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void*)listener);
	return listener;
}

dimensionListener_t* widget_addDimensionListener(void *widget,
	dimensionEventType_t dimType, void(*listenerFunc)(void *widget, dimensionEvent_t *event))
{
	dimensionListener_t *listener = dimensionListener_new(widget);
	((eventListener_t*)listener)->listenerType = EV_Dimension;
	listener->type = dimType;
	listener->types.listen = listenerFunc;
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void*)listener);
	return listener;
}

lifecycleListener_t* widget_addLifeCycleListener(void *widget,
	lifecycleEventType_t lifeType, void(*listenerFunc)(void *widget, lifecycleEvent_t *event))
{
	lifecycleListener_t *listener = new(lifecycleListener_t);
	((eventListener_t*)listener)->listenerType = EV_LifeCycle;
	listener->type = lifeType;
	listener->types.listen = listenerFunc;
	list_push_back(((guiWidget_t *)widget)->eventListeners, (void*)listener);
	return listener;
}

void widget_removeListener(void *widget, eventListener_t *listener)
{
	list_remove(((guiWidget_t*)widget)->eventListeners, (void*)listener);
}

void widget_tick(guiWidget_t *widget)
{
	return;
}

void widget_draw(const guiWidget_t *widget, guiGraphics_t *graphics)
{
	return;
}

void widget_init(guiWidget_t* widget)
{
	guiDebugPrint("trying to create widget");
	guiRectangle_t dim = {0, 0, 0, 0};
	widget->dim = dim;
	widget->flags = WF_ENABLED + WF_VISIBLE;
	widget->eventListeners = list_new();
	widget->font = NULL;
	widget->parent = NULL;
	widget->v = &guiWidget_vtable;
	
	guiDebugPrint("new widget created");
}

guiRectangle_t* widget_getChildrenArea(const guiWidget_t *widget)
{
	return &(guiRectangle_t){0, 0, 0, 0};
}

guiWidget_t* widget_getWidgetAt(const guiWidget_t* widget, vec2i_t pos)
{
	return NULL;
}

bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist)
{
	return widget == exist ? true : false;
}

void widget_setDimension(guiWidget_t* widget, const guiRectangle_t newDim)
{
	guiRectangle_t oldDim = widget->dim;
	widget->dim = newDim;
	if (widget->dim.width != oldDim.width || widget->dim.height != oldDim.height) {
		dimensionEvent_t *event = new(dimensionEvent_t);
		((event_t*)event)->eventType = EV_Dimension;
		event->pos = newDim.pos;
		event->width = newDim.width;
		event->height = newDim.height;
		event->type = DE_RESIZED;
		widget_handleEvent((guiWidget_t*)widget, (event_t*)event);
	}
	if (widget->dim.pos.x != oldDim.pos.x || widget->dim.pos.y != oldDim.pos.y) {
		dimensionEvent_t *event = new(dimensionEvent_t);
		((event_t*)event)->eventType = EV_Dimension;
		event->pos = newDim.pos;
		event->width = newDim.width;
		event->height = newDim.height;
		event->type = DE_MOVED;
		widget_handleEvent((guiWidget_t*)widget, (event_t*)event);
	}
	guiDebugPrint("dimension set");
}

void widget_getAbsolutePosition(guiWidget_t* widget, vec2i_t *pos)
{
	if (!widget->parent){ 
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

void widget_handleEvent(guiWidget_t* widget, event_t* event)
{
	eventListener_t *listener;
	for (listNode_t *node = widget->eventListeners->head; node; node = node->next) {
		listener = (eventListener_t*)node->data;
		if (listener->listenerType == event->eventType) {
			listener->handleEvent(listener, event);
		}
	}
}
