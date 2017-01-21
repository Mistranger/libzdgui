#include "system.h"

#include "widgets/button.h"

#include "event.h"
#include "graphics.h"

guiButton_vf_t guiButton_vtable = {
	widget_getChildrenArea,
	widget_getWidgetAt,
	button_draw,
	widget_tick,
	widget_isWidgetExisting
};

#define button_addMouseListener(eventType, mouseType, listenerFunc) \
{\
	eventListener_t *listener = (eventListener_t*)malloc(sizeof(eventListener_t));\
	listener->type = eventType;\
	mouseListener_t *mouseListener = (mouseListener_t*)malloc(sizeof(mouseListener_t));\
	mouseListener->type = mouseType;\
	mouseListener->types.mousePressed = listenerFunc;\
	listener->listeners.mouseListener = mouseListener;\
	list_push_back(button->widget.eventListeners, (void*)listener);\
}

guiButton_t* button_new(const char* caption)
{
	guiButton_t *button = (guiButton_t*)malloc(sizeof(guiButton_t));
	button_init(button);
	button_setCaption(button, caption);
	button_adjustSize(button);
	return button;
}

void button_init(guiButton_t* button)
{
	widget_init(&button->widget);
	
	button->widget.v = (guiWidget_vf_t*)&guiButton_vtable;

	button->widget.flags |= WF_FOCUSABLE;
	button->buttonFlags = 0;
	
	// Fill listener list
	button_addMouseListener(EV_Mouse, ME_PRESSED, button_mousePressed);
	button_addMouseListener(EV_Mouse, ME_RELEASED, button_mouseReleased);	
	button_addMouseListener(EV_Mouse, ME_ENTERED, button_mouseEntered);	
	button_addMouseListener(EV_Mouse, ME_LEFT, button_mouseLeft);	
}

void button_draw(const guiButton_t* button, guiGraphics_t* graphics)
{
	guiDebugPrint("drawing button");
	graph_drawText(graphics, 
		widget_getX((guiWidget_t*)(&button->widget)),
		widget_getY((guiWidget_t*)(&button->widget)),
		button_getCaption(button));
}

void button_setCaption(guiButton_t* button, const char* caption)
{
	strncpy(button->caption, caption, MAX_CAPTION_LENGTH);
}

const char* button_getCaption(const guiButton_t* button)
{
	return button->caption;
}

void button_adjustSize(guiButton_t* button)
{
	guiDebugPrint("new button size: %d %d" _C_ 8*strlen(button_getCaption(button)) + 10 _C_ 20);
	widget_setSize((guiWidget_t*)button, 8* strlen(button_getCaption(button)) + 10, 20);
}

void button_mousePressed(void *widget, mouseEvent_t* mouseEvent)
{
	guiDebugPrint("button pressed");
	guiButton_t *button = (guiButton_t *)widget;
	if (mouseEvent->button == LEFT) {
		button->buttonFlags |= BF_PRESSED;
	}
}

void button_mouseReleased(void *widget, mouseEvent_t* mouseEvent)
{
	guiDebugPrint("button released");
	guiButton_t *button = (guiButton_t *)widget;
	if (mouseEvent->button == LEFT) {
		if ((button->buttonFlags & BF_PRESSED) && (button->buttonFlags & BF_HASMOUSE)) {
			button->buttonFlags &= ~BF_PRESSED;
			event_t *event = (event_t*)malloc(sizeof(event_t));
			event->type = EV_Mouse;
			mouseEvent_t *clicked = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
			*clicked = *mouseEvent;
			event->events.mouse = clicked;
			widget_handleEvent((guiWidget_t*)widget, event);
		} else {
			button->buttonFlags &= ~BF_PRESSED;
		}
	}
}

void button_mouseEntered(void *widget, mouseEvent_t* mouseEvent)
{
	guiButton_t *button = (guiButton_t *)widget;
	button->buttonFlags |= BF_HASMOUSE;
}

void button_mouseLeft(void *widget, mouseEvent_t* mouseEvent)
{
	guiButton_t *button = (guiButton_t *)widget;
	button->buttonFlags &= ~BF_HASMOUSE;
}
