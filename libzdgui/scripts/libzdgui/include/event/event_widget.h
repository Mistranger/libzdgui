#ifndef EVENT_WIDGET_H_INCLUDED
#define EVENT_WIDGET_H_INCLUDED

#include "dimension.h"
#include "event.h"
#include "widget.h"

typedef enum widgetEventType {
	WE_VALUE_CHANGED = 0,
	
} widgetEventType_t;

typedef struct widgetEvent_s {
	event_t event;
	widgetEventType_t type;
} widgetEvent_t;

typedef struct widgetListener_s {
	eventListener_t listener;
	union {
		void (*listen)(void *widget, widgetEvent_t *widgetEvent);
		void (*valueChanged)(void *widget, widgetEvent_t *widgetEvent);
		
	} types;
	widgetEventType_t type;
} widgetListener_t;

widgetEvent_t* widgetEvent_new(void *source, widgetEventType_t eventType);

widgetListener_t* widgetListener_new(void *handler);
void widgetListener_handleEvent(eventListener_t *listener, event_t *event);



#endif // EVENT_MOUSE_H_INCLUDED
