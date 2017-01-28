#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

// Union holding all event types

typedef enum eventTypes {
	EV_Event = 0,
	EV_Mouse,
	EV_Dimension,
	EV_LifeCycle,
	EV_Widget,
	EV_Focus,
} guiEventTypes;

typedef struct guiEvent {
	guiEventTypes eventType;
	void *sourceWidget;
} guiEvent;

#define event_getSource(_event) ((guiWidget*)(((guiEvent*)_event)->sourceWidget))
#define event_setSource(_event, _widget) { ((guiEvent*)_event)->sourceWidget = (void*)_widget; }

guiEvent *event_new(void *widget);

// Listeners

// Union holding all event listeners
// One listener could handle one event type
// Widgets could have multiple listeners for various event types
typedef struct guiEventListener {
	guiEventTypes listenerType;
	void *handlerWidget;
	void (*handleEvent)(struct guiEventListener *listener, guiEvent *event);
} guiEventListener;


#endif // EVENT_H_INCLUDED
