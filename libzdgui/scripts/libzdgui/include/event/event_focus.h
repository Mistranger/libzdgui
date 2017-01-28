#ifndef EVENT_FOCUS_H_INCLUDED
#define EVENT_FOCUS_H_INCLUDED

#include "event.h"

typedef enum focusEventType {
	FE_FOCUSGAINED = 0,
	FE_FOCUSLOST
} focusEventType_t;

typedef struct focusEvent_s {
	event_t event;
	focusEventType_t type;
} focusEvent_t;

typedef struct focusListener_s {
	eventListener_t listener;
	union {
		void (*listen)(void *widget, focusEvent_t *mouseEvent);
		void (*focusGained)(void *widget, focusEvent_t *focusEvent);
		void (*focusLost)(void *widget, focusEvent_t *focusEvent);
	} types;
	focusEventType_t type;
} focusListener_t;

focusEvent_t *focusEvent_new(void *source, int eventType);

focusListener_t *focusListener_new(void *handler, focusEventType_t eventType, void (*func)(void *widget, focusEvent_t *focusEvent));
void focus_handleEvent(eventListener_t *listener, event_t *event);

#endif // EVENT_FOCUS_H_INCLUDED