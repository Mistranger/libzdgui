#ifndef EVENT_LIFECYCLE_H_INCLUDED
#define EVENT_LIFECYCLE_H_INCLUDED

#include "event.h"

typedef enum lifecycleEventType {
	LE_CREATED = 0,
	LE_DESTROYED
} lifecycleEventType_t;

typedef struct lifecycleEvent_s {
	event_t event;
	lifecycleEventType_t type;
} lifecycleEvent_t;

typedef struct lifecycleListener_s {
	eventListener_t listener;
	union {
		void (*listen)(void *widget, lifecycleEvent_t *lifeEvent);
		void (*created)(void *widget, lifecycleEvent_t *lifeEvent);
		void (*destroyed)(void *widget, lifecycleEvent_t *lifeEvent);
	} types;
	lifecycleEventType_t type;
} lifecycleListener_t;

lifecycleListener_t* lifecycleListener_new(void *handler);
void lifecycle_handleEvent(eventListener_t *listener, event_t *event);

#endif // EVENT_LIFECYCLE_H_INCLUDED