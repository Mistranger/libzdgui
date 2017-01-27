#include "system.h"
#include "event/event_mouse.h"

mouseEvent_t* mouseEvent_new(void* source, const vec2i_t* pos, mouseButtons_t button, mouseEventType_t eventType)
{
	mouseEvent_t *event = new(mouseEvent_t);
	((event_t*)event)->eventType = EV_Mouse;
	((event_t*)event)->sourceWidget = source;
	event->pos = *pos;
	event->button = button;
	event->type = eventType;
	return event;
}

mouseListener_t* mouseListener_new(void *handler, mouseEventType_t eventType, void (*func)(void *widget, mouseEvent_t *mouseEvent))
{
	mouseListener_t *listener = new(mouseListener_t);
	listener->listener.listenerType = EV_Mouse;
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = mouse_handleEvent;
	listener->type = eventType;
	listener->types.listen = func;
	return listener;
}

void mouse_handleEvent(eventListener_t *listener, event_t *event)
{
	mouseListener_t *mouseL = (mouseListener_t*)listener;
	mouseEvent_t *mouseE = (mouseEvent_t*)event;
	if (mouseL->type == mouseE->type) {
		switch (mouseL->type) {
			case ME_MOVED:
				mouseL->types.mouseMoved((void*)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_PRESSED:
				mouseL->types.mousePressed((void*)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_RELEASED:
				mouseL->types.mouseReleased((void*)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_ENTERED:
				mouseL->types.mouseEntered((void*)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_LEFT:
				mouseL->types.mouseEntered((void*)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_CLICKED:
				mouseL->types.mouseClicked((void*)mouseL->listener.handlerWidget, mouseE);
				break;
			case ME_DRAGGED:
				mouseL->types.mouseDragged((void*)mouseL->listener.handlerWidget, mouseE);
				break;
			default:
				guiError("Unknown mouse event!");
				break;
			
		}
	}
}

