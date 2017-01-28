#include "system.h"
#include "event/event_dimension.h"

guiDimensionEvent *dimensionEvent_new(void *source, const vec2i *pos, int width, int height, int eventType)
{
	guiDimensionEvent *event = new (guiDimensionEvent);
	((guiEvent *)event)->eventType = EV_Dimension;
	((guiEvent *)event)->sourceWidget = source;
	event->pos = *pos;
	event->width = width;
	event->height = height;
	event->type = eventType;
	return event;
}

dimensionListener_t *dimensionListener_new(void *handler, guiDimensionEventType eventType, void (*func)(void *widget, guiDimensionEvent *dimEvent))
{
	dimensionListener_t *listener = new (dimensionListener_t);
	listener->listener.listenerType = EV_Dimension;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = dimension_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void dimension_handleEvent(guiEventListener *listener, guiEvent *event)
{
	dimensionListener_t *dimL = (dimensionListener_t *)listener;
	guiDimensionEvent *dimE = (guiDimensionEvent *)event;
	if (dimL->type == dimE->type) {
		switch (dimL->type) {
			case DE_MOVED:
				dimL->types.moved(listener->handlerWidget, dimE);
				break;
			case DE_RESIZED:
				dimL->types.resized(listener->handlerWidget, dimE);
				break;
			default:
				guiError("Unknown event!");
				break;
		}
	}
}
