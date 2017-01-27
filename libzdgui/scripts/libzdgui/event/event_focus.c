#include "system.h"
#include "event/event_focus.h"

focusEvent_t* focusEvent_new(void* source, int eventType)
{
	focusEvent_t *event = new(focusEvent_t);
	((event_t*)event)->eventType = EV_Focus;
	((event_t*)event)->sourceWidget = source;
	event->type = eventType;
	return event;
}

focusListener_t* focusListener_new(void *handler, focusEventType_t eventType, void (*func)(void *widget, focusEvent_t *focusEvent))
{
	focusListener_t *listener = new(focusListener_t);
	listener->listener.listenerType = EV_Focus;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = focus_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void focus_handleEvent(eventListener_t *listener, event_t *event)
{
	focusListener_t *focusL = (focusListener_t*)listener;
	focusEvent_t *focusE = (focusEvent_t*)event;
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
