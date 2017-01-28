#ifndef EVENT_WIDGET_H_INCLUDED
#define EVENT_WIDGET_H_INCLUDED

#include "dimension.h"
#include "event.h"
#include "widget.h"

typedef enum widgetEventType {
	WE_VALUE_CHANGED = 0,

} guiWidgetEventType;

typedef struct guiWidgetEvent {
	guiEvent event;
	guiWidgetEventType type;
} guiWidgetEvent;

typedef struct guiWidgetListener {
	guiEventListener listener;
	union {
		void (*listen)(void *widget, guiWidgetEvent *widgetEvent);
		void (*valueChanged)(void *widget, guiWidgetEvent *widgetEvent);

	} types;
	guiWidgetEventType type;
} guiWidgetListener;

guiWidgetEvent *widgetEvent_new(void *source, guiWidgetEventType eventType);

guiWidgetListener *widgetListener_new(void *handler, guiWidgetEventType eventType, void (*func)(void *widget, guiWidgetEvent *mouseEvent));
void widgetListener_handleEvent(guiEventListener *listener, guiEvent *event);



#endif // EVENT_MOUSE_H_INCLUDED
