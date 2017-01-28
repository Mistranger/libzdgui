#include "system.h"
#include "event/event_focus.h"

guiFocusEvent *focusEvent_new(void *source, int eventType)
{
	guiFocusEvent *event = new (guiFocusEvent);
	((guiEvent *)event)->eventType = EV_Focus;
	((guiEvent *)event)->sourceWidget = source;
	event->type = eventType;
	return event;
}

guiFocusListener *focusListener_new(void *handler, guiFocusEventType eventType, void (*func)(void *widget, guiFocusEvent *focusEvent))
{
	guiFocusListener *listener = new (guiFocusListener);
	listener->listener.listenerType = EV_Focus;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = focus_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void focus_handleEvent(guiEventListener *listener, guiEvent *event)
{
	guiFocusListener *focusL = (guiFocusListener *)listener;
	guiFocusEvent *focusE = (guiFocusEvent *)event;
	if (focusL->type == focusE->type) {
		switch (focusL->type) {
			case FE_FOCUSGAINED:
				focusL->types.focusGained(listener->handlerWidget, focusE);
				break;
			case FE_FOCUSLOST:
				focusL->types.focusLost(listener->handlerWidget, focusE);
				break;
			default:
				guiError("Unknown event!");
				break;
		}
	}
}
