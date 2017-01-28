#include "system.h"
#include "event/event_widget.h"

guiWidgetEvent *widgetEvent_new(void *source, guiWidgetEventType eventType)
{
	guiWidgetEvent *event = new (guiWidgetEvent);
	((guiEvent *)event)->eventType = EV_Widget;
	((guiEvent *)event)->sourceWidget = source;
	event->type = eventType;
	return event;
}

guiWidgetListener *widgetListener_new(void *handler, guiWidgetEventType eventType, void (*func)(void *widget, guiWidgetEvent *mouseEvent))
{
	guiWidgetListener *listener = new (guiWidgetListener);
	listener->listener.listenerType = EV_Widget;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = widgetListener_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void widgetListener_handleEvent(guiEventListener *listener, guiEvent *event)
{
	guiWidgetListener *widgetL = (guiWidgetListener *)listener;
	guiWidgetEvent *widgetE = (guiWidgetEvent *)event;
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
