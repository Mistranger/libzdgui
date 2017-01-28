#include "system.h"
#include "event/event_lifecycle.h"

lifecycleEvent_t *lifecycleEvent_new(void *source, lifecycleEventType_t eventType)
{
	lifecycleEvent_t *event = new (lifecycleEvent_t);
	((event_t *)event)->eventType = EV_LifeCycle;
	((event_t *)event)->sourceWidget = source;
	event->type = eventType;
	return event;
}

lifecycleListener_t *lifecycleListener_new(void *handler, lifecycleEventType_t eventType, void (*func)(void *widget, lifecycleEvent_t *lifecycleEvent))
{
	lifecycleListener_t *listener = new (lifecycleListener_t);
	listener->listener.listenerType = EV_LifeCycle;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = lifecycle_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void lifecycle_handleEvent(eventListener_t *listener, event_t *event)
{
	lifecycleListener_t *lifecycleL = (lifecycleListener_t *)listener;
	lifecycleEvent_t *lifecycleE = (lifecycleEvent_t *)event;
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

