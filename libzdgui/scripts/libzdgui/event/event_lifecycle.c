#include "system.h"
#include "event/event_lifecycle.h"

lifecycleListener_t* lifecycleListener_new(void *handler)
{
	lifecycleListener_t *listener = new(lifecycleListener_t);
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = lifecycle_handleEvent;
	return listener;
}

void lifecycle_handleEvent(eventListener_t *listener, event_t *event)
{
	lifecycleListener_t *lifecycleL = (lifecycleListener_t*)listener;
	lifecycleEvent_t *lifecycleE = (lifecycleEvent_t*)event;
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

