#ifndef EVENT_DIMENSION_H_INCLUDED
#define EVENT_DIMENSION_H_INCLUDED


typedef enum dimensionEventType {
	DE_MOVED = 0,
	DE_RESIZED
} dimensionEventType_t;

typedef struct dimensionEvent_s {
	int x, y;
	int width, height;
	dimensionEventType_t type;
} dimensionEvent_t;

typedef struct dimensionListener_s {
	union {
		void (*resized)(void *widget, dimensionEvent_t *dimEvent);
		void (*moved)(void *widget, dimensionEvent_t *dimEvent);
	} types;
	dimensionEventType_t type;
} dimensionListener_t;



#endif // EVENT_DIMENSION_H_INCLUDED