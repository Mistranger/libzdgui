#ifndef EVENT_MOUSE_H_INCLUDED
#define EVENT_MOUSE_H_INCLUDED

enum mouseButtons {
	EMPTY = 0x00,
	LEFT = 0x01,
	RIGHT = 0x02,
	MIDDLE = 0x04
};

typedef enum mouseEventType {
	ME_MOVED = 0,
	ME_PRESSED,
	ME_RELEASED,
	ME_ENTERED,
	ME_LEFT,
	ME_CLICKED
} mouseEventType_t;

typedef struct mouseEvent_s {
	int x, y;
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
