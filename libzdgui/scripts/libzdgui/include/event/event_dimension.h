#ifndef EVENT_DIMENSION_H_INCLUDED
#define EVENT_DIMENSION_H_INCLUDED

#include "dimension.h"
#include "event.h"

typedef enum dimensionEventType {
	DE_MOVED = 0,
	DE_RESIZED
} guiDimensionEventType;

typedef struct guiDimensionEvent {
	guiEvent event;
	vec2i pos;
	int width, height;
	guiDimensionEventType type;
} guiDimensionEvent;

typedef struct dimensionListener_s {
	guiEventListener listener;
	union {
		void (*listen)(void *widget, guiDimensionEvent *dimEvent);
		void (*resized)(void *widget, guiDimensionEvent *dimEvent);
		void (*moved)(void *widget, guiDimensionEvent *dimEvent);
	} types;
	guiDimensionEventType type;
} dimensionListener_t;

guiDimensionEvent *dimensionEvent_new(void *source, const vec2i *pos, int width, int height, int eventType);

dimensionListener_t *dimensionListener_new(void *handler, guiDimensionEventType eventType, void (*func)(void *widget, guiDimensionEvent *dimEvent));
void dimension_handleEvent(guiEventListener *listener, guiEvent *event);

#endif // EVENT_DIMENSION_H_INCLUDED