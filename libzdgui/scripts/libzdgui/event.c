#include "system.h"

#include "event.h"

guiEvent *event_new(void *widget)
{
	guiEvent *event = new (guiEvent);
	event->eventType = EV_Event;
	event->sourceWidget = widget;
	return event;
}