#include "system.h"
#include "event/event_mouse.h"

guiMouseEvent *mouseEvent_new(void *source, const vec2i *pos, guiMouseButtons button, mouseEventType_t eventType)
{
	guiMouseEvent *event = new (guiMouseEvent);
	((guiEvent *)event)->eventType = EV_Mouse;
	((guiEvent *)event)->sourceWidget = source;
	event->pos = *pos;
	event->button = button;
	event->type = eventType;
	return event;
}

guiMouseListener *mouseListener_new(void *handler, mouseEventType_t eventType, void (*func)(void *widget, guiMouseEvent *mouseEvent))
{
	guiMouseListener *listener = new (guiMouseListener);
	listener->listener.listenerType = EV_Mouse;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = mouse_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void mouse_handleEvent(guiEventListener *listener, guiEvent *event)
{
	guiMouseListener *mouseL = (guiMouseListener *)listener;
	guiMouseEvent *mouseE = (guiMouseEvent *)event;
	if (mouseL->type == mouseE->type) {
		switch (mouseL->type) {
			case ME_MOVED:
				mouseL->types.mouseMoved((void *)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_PRESSED:
				mouseL->types.mousePressed((void *)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_RELEASED:
				mouseL->types.mouseReleased((void *)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_ENTERED:
				mouseL->types.mouseEntered((void *)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_LEFT:
				mouseL->types.mouseEntered((void *)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_CLICKED:
				mouseL->types.mouseClicked((void *)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_DRAGGED:
				mouseL->types.mouseDragged((void *)mouseL->listener.handlerWidget, mouseE);
				break;
			default:
				guiError("Unknown mouse event!");
				break;

		}
	}
}

