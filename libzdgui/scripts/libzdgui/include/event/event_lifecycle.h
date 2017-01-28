#ifndef EVENT_LIFECYCLE_H_INCLUDED
#define EVENT_LIFECYCLE_H_INCLUDED

#include "event.h"

typedef enum lifecycleEventType {
	LE_CREATED = 0,
	LE_DESTROYED
} guiLifecycleEventType;

typedef struct guiLifecycleEvent {
	guiEvent event;
	guiLifecycleEventType type;
} guiLifecycleEvent;

typedef struct guiLifecycleListener {
	guiEventListener listener;
	union {
		void (*listen)(void *widget, guiLifecycleEvent *lifeEvent);
		void (*created)(void *widget, guiLifecycleEvent *lifeEvent);
		void (*destroyed)(void *widget, guiLifecycleEvent *lifeEvent);
	} types;
	guiLifecycleEventType type;
} guiLifecycleListener;

guiLifecycleEvent *lifecycleEvent_new(void *source, guiLifecycleEventType eventType);

guiLifecycleListener *lifecycleListener_new(void *handler, guiLifecycleEventType eventType, void (*func)(void *widget, guiLifecycleEvent *lifecycleEvent));
void lifecycle_handleEvent(guiEventListener *listener, guiEvent *event);

#endif // EVENT_LIFECYCLE_H_INCLUDED