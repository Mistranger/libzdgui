#ifndef EVENT_FOCUS_H_INCLUDED
#define EVENT_FOCUS_H_INCLUDED

#include "event.h"

typedef enum focusEventType {
	FE_FOCUSGAINED = 0,
	FE_FOCUSLOST
} guiFocusEventType;

typedef struct guiFocusEvent {
	guiEvent event;
	guiFocusEventType type;
} guiFocusEvent;

typedef struct guiFocusListener {
	guiEventListener listener;
	union {
		void (*listen)(void *widget, guiFocusEvent *mouseEvent);
		void (*focusGained)(void *widget, guiFocusEvent *focusEvent);
		void (*focusLost)(void *widget, guiFocusEvent *focusEvent);
	} types;
	guiFocusEventType type;
} guiFocusListener;

guiFocusEvent *focusEvent_new(void *source, int eventType);

guiFocusListener *focusListener_new(void *handler, guiFocusEventType eventType, void (*func)(void *widget, guiFocusEvent *focusEvent));
void focus_handleEvent(guiEventListener *listener, guiEvent *event);

#endif // EVENT_FOCUS_H_INCLUDED