#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

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

typedef enum dimensionEventType {
	DE_MOVED = 0,
	DE_RESIZED
} dimensionEventType_t;

typedef struct dimensionEvent_s {
	int x, y;
	int width, height;
	dimensionEventType_t type;
} dimensionEvent_t;

// Union holding all event types

typedef enum eventTypes {
	EV_Mouse = 0,
	EV_Dimension,
} eventTypes_t;

typedef struct event_s
{
	union {
		mouseEvent_t *mouse;
		dimensionEvent_t *dimension;
	} events;
	eventTypes_t type;
} event_t;


// Listeners
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

typedef struct dimensionListener_s {
	union {
		void (*resized)(void *widget, dimensionEvent_t *dimEvent);
		void (*moved)(void *widget, dimensionEvent_t *dimEvent);
	} types;
	dimensionEventType_t type;
} dimensionListener_t;

// Union holding all event listeners
// One listener could handle one event type
// Widgets could have multiple listeners for various event types
typedef struct eventListener_s {
	union {
		// EV_Mouse
		mouseListener_t *mouseListener;
		
		dimensionListener_t *dimensionListener;
		
	} listeners;
	eventTypes_t type;
} eventListener_t;


#endif // EVENT_H_INCLUDED
