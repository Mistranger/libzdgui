#include "system.h"
#include "event/event_widget.h"

widgetEvent_t* widgetEvent_new(void *source, widgetEventType_t eventType)
{
	widgetEvent_t *event = new(widgetEvent_t);
	((event_t*)event)->eventType = EV_Widget;
	((event_t*)event)->sourceWidget = source;
	event->type = eventType;
	return event;
}

widgetListener_t* widgetListener_new(void *handler, widgetEventType_t eventType, void (*func)(void *widget, widgetEvent_t *mouseEvent))
{
	widgetListener_t *listener = new(widgetListener_t);
	listener->listener.listenerType = EV_Widget;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = widgetListener_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void widgetListener_handleEvent(eventListener_t *listener, event_t *event)
{
	widgetListener_t *widgetL = (widgetListener_t*)listener;
	widgetEvent_t *widgetE = (widgetEvent_t*)event;
	if (widgetL->type == widgetE->type) {
		switch (widgetL->type) {
		case WE_VALUE_CHANGED:
			widgetL->types.valueChanged(listener->handlerWidget, widgetE);
			break;
		default:
			guiError("Unknown event!");
			break;
		}
	}
}
