#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include "event/event_dimension.h"
#include "event/event_mouse.h"

// Union holding all event types

typedef enum eventTypes {
	EV_Event = 0,
	EV_Mouse,
	EV_Dimension,
} eventTypes_t;

typedef struct event_s
{
	union {
		mouseEvent_t *mouse;
		dimensionEvent_t *dimension;
	} events;
	eventTypes_t type;
} event_t;


// Listeners

// Union holding all event listeners
// One listener could handle one event type
// Widgets could have multiple listeners for various event types
typedef struct eventListener_s {
	union {
		// EV_Mouse
		mouseListener_t *mouseListener;
		dimensionListener_t *dimensionListener;
	} listeners;
	eventTypes_t type;
} eventListener_t;


#endif // EVENT_H_INCLUDED
