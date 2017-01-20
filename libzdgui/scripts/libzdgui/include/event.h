#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

enum mouseButtons {
	EMPTY = 0x00,
	LEFT = 0x01,
	RIGHT = 0x02,
	MIDDLE = 0x04
};

enum mouseEventType {
	MOVED = 0,
	PRESSED,
	RELEASED
} ;

typedef struct mouseEvent_s {
	int x, y;
	enum mouseButtons button;
	enum mouseEventType type;
} mouseEvent_t;


// Union holding all event types

typedef enum eventTypes {
	EV_Mouse = 0,
} eventTypes_t;

typedef struct event_s
{
	union {
		mouseEvent_t *mouse;
	} events;
	eventTypes_t type;
} event_t;


#endif // EVENT_H_INCLUDED
