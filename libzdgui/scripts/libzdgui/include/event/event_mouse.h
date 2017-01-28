#ifndef EVENT_MOUSE_H_INCLUDED
#define EVENT_MOUSE_H_INCLUDED

#include "dimension.h"
#include "event.h"
#include "mouse.h"

typedef enum mouseEventType {
	ME_MOVED = 0,
	ME_PRESSED,
	ME_RELEASED,
	ME_ENTERED,
	ME_LEFT,
	ME_CLICKED,
	ME_DRAGGED,
} mouseEventType_t;

typedef struct guiMouseEvent {
	guiEvent event;
	vec2i pos;
	guiMouseButtons button;
	mouseEventType_t type;
} guiMouseEvent;

typedef struct guiMouseListener {
	guiEventListener listener;
	union {
		void (*listen)(void *widget, guiMouseEvent *mouseEvent);
		void (*mouseMoved)(void *widget, guiMouseEvent *mouseEvent);
		void (*mousePressed)(void *widget, guiMouseEvent *mouseEvent);
		void (*mouseReleased)(void *widget, guiMouseEvent *mouseEvent);
		void (*mouseEntered)(void *widget, guiMouseEvent *mouseEvent);
		void (*mouseLeft)(void *widget, guiMouseEvent *mouseEvent);
		void (*mouseClicked)(void *widget, guiMouseEvent *mouseEvent);
		void (*mouseDragged)(void *widget, guiMouseEvent *mouseEvent);
	} types;
	mouseEventType_t type;
} guiMouseListener;

guiMouseEvent *mouseEvent_new(void *source, const vec2i *pos, guiMouseButtons button, mouseEventType_t eventType);

guiMouseListener *mouseListener_new(void *handler, mouseEventType_t eventType, void (*func)(void *widget, guiMouseEvent *mouseEvent));
void mouse_handleEvent(guiEventListener *listener, guiEvent *event);



#endif // EVENT_MOUSE_H_INCLUDED
