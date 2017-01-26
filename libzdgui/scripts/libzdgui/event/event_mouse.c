#include "system.h"
#include "event/event_mouse.h"

mouseListener_t* mouseListener_new(void *handler)
{
	mouseListener_t *listener = new(mouseListener_t);
	listener->listener.handlerWidget = handler;
	listener->listener.handleEvent = mouse_handleEvent;
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
		}
	}
}

