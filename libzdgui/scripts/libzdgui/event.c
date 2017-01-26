#include "system.h"

#include "event.h"

event_t* event_new(void *widget)
{
	event_t *event = new(event_t);
	event->eventType = EV_Event;
	event->sourceWidget = widget;
	return event;
}