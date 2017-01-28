#include "system.h"
#include "event/event_lifecycle.h"

guiLifecycleEvent *lifecycleEvent_new(void *source, guiLifecycleEventType eventType)
{
	guiLifecycleEvent *event = new (guiLifecycleEvent);
	((guiEvent *)event)->eventType = EV_LifeCycle;
	((guiEvent *)event)->sourceWidget = source;
	event->type = eventType;
	return event;
}

guiLifecycleListener *lifecycleListener_new(void *handler, guiLifecycleEventType eventType, void (*func)(void *widget, guiLifecycleEvent *lifecycleEvent))
{
	guiLifecycleListener *listener = new (guiLifecycleListener);
	listener->listener.listenerType = EV_LifeCycle;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = lifecycle_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void lifecycle_handleEvent(guiEventListener *listener, guiEvent *event)
{
	guiLifecycleListener *lifecycleL = (guiLifecycleListener *)listener;
	guiLifecycleEvent *lifecycleE = (guiLifecycleEvent *)event;
	if (lifecycleL->type == lifecycleE->type) {
		switch (lifecycleL->type) {
			case LE_CREATED:
				lifecycleL->types.created(listener->handlerWidget, lifecycleE);
				break;
			case LE_DESTROYED:
				lifecycleL->types.destroyed(listener->handlerWidget, lifecycleE);
				break;
			default:
				break;
		}
	}
}

