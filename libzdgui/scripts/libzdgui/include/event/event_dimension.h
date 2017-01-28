#ifndef EVENT_DIMENSION_H_INCLUDED
#define EVENT_DIMENSION_H_INCLUDED

#include "dimension.h"
#include "event.h"

typedef enum dimensionEventType {
	DE_MOVED = 0,
	DE_RESIZED
} dimensionEventType_t;

typedef struct dimensionEvent_s {
	event_t event;
	vec2i_t pos;
	int width, height;
	dimensionEventType_t type;
} dimensionEvent_t;

typedef struct dimensionListener_s {
	eventListener_t listener;
	union {
		void (*listen)(void *widget, dimensionEvent_t *dimEvent);
		void (*resized)(void *widget, dimensionEvent_t *dimEvent);
		void (*moved)(void *widget, dimensionEvent_t *dimEvent);
	} types;
	dimensionEventType_t type;
} dimensionListener_t;

dimensionEvent_t *dimensionEvent_new(void *source, const vec2i_t *pos, int width, int height, int eventType);

dimensionListener_t *dimensionListener_new(void *handler, dimensionEventType_t eventType, void (*func)(void *widget, dimensionEvent_t *dimEvent));
void dimension_handleEvent(eventListener_t *listener, event_t *event);

#endif // EVENT_DIMENSION_H_INCLUDED