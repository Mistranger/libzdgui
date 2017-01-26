#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

// Union holding all event types

typedef enum eventTypes {
	EV_Event = 0,
	EV_Mouse,
	EV_Dimension,
	EV_LifeCycle,
} eventTypes_t;

typedef struct event_s {
	eventTypes_t eventType;
	void *sourceWidget;
} event_t;

#define event_getSource(_event) ((guiWidget_t*)(((event_t*)_event)->sourceWidget))

// Listeners

// Union holding all event listeners
// One listener could handle one event type
// Widgets could have multiple listeners for various event types
typedef struct eventListener_s {
	eventTypes_t listenerType;
	void *handlerWidget;
	void (*handleEvent)(struct eventListener_s *listener, event_t *event);
} eventListener_t;


#endif // EVENT_H_INCLUDED
