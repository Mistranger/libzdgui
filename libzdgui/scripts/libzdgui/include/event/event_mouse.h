#ifndef EVENT_MOUSE_H_INCLUDED
#define EVENT_MOUSE_H_INCLUDED

#include "dimension.h"
#include "mouse.h"

typedef enum mouseEventType {
	ME_MOVED = 0,
	ME_PRESSED,
	ME_RELEASED,
	ME_ENTERED,
	ME_LEFT,
	ME_CLICKED
} mouseEventType_t;

typedef struct mouseEvent_s {
	vec2i_t pos;
	enum mouseButtons button;
	mouseEventType_t type;
} mouseEvent_t;

typedef struct mouseListener_s {
	union {
		void (*mouseMoved)(void *widget, mouseEvent_t *mouseEvent);
		void (*mousePressed)(void *widget, mouseEvent_t *mouseEvent);
		void (*mouseReleased)(void *widget, mouseEvent_t *mouseEvent);
		void (*mouseEntered)(void *widget, mouseEvent_t *mouseEvent);
		void (*mouseLeft)(void *widget, mouseEvent_t *mouseEvent);
		void (*mouseClicked)(void *widget, mouseEvent_t *mouseEvent);
	} types;
	mouseEventType_t type;
} mouseListener_t;

#endif // EVENT_MOUSE_H_INCLUDED
