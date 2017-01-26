#include "system.h"
#include "event/event_dimension.h"

dimensionEvent_t* dimensionEvent_new(void* source, const vec2i_t* pos, int width, int height, int eventType)
{
	dimensionEvent_t *event = new(dimensionEvent_t);
	((event_t*)event)->eventType = EV_Dimension;
	((event_t*)event)->sourceWidget = source;
	event->pos = *pos;
	event->width = width;
	event->height = height;
	event->type = eventType;
	return event;
}

dimensionListener_t* dimensionListener_new(void *handler)
{
	dimensionListener_t *listener = new(dimensionListener_t);
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = dimension_handleEvent;
	return listener;
}

void dimension_handleEvent(eventListener_t *listener, event_t *event)
{
	dimensionListener_t *dimL = (dimensionListener_t*)listener;
	dimensionEvent_t *dimE = (dimensionEvent_t*)event;
	if (dimL->type == dimE->type) {
		switch (dimL->type) {
		case DE_MOVED:
			dimL->types.moved(listener->handlerWidget, dimE);
			break;
		case DE_RESIZED:
			dimL->types.resized(listener->handlerWidget, dimE);
			break;
		}
	}
}
