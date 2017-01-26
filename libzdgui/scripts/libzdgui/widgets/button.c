#include "system.h"

#include "widgets/button.h"

#include "event.h"
#include "fonts.h"
#include "graphics.h"

guiButton_vf_t guiButton_vtable = {
	button_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	button_draw,
	widget_tick,
	widget_isWidgetExisting
};

guiButton_t* button_new(guiGUI_t *gui, const string_t* caption)
{
	guiButton_t *button = new(guiButton_t);
	button_init(button, caption);
	gui_addWidget(gui, button);
	return button;
}

void button_destructor(guiButton_t *button)
{
	string_delete(button->caption);
	widget_destructor((guiWidget_t*)button);
}

void button_init(guiButton_t* button, const string_t* caption)
{
	widget_init(&button->widget);
	
	button->widget.v = (guiWidget_vf_t*)&guiButton_vtable;

	widget_setFont(button, (guiFont_t*)&FONT_CONFONT);
	button->widget.flags |= WF_FOCUSABLE;
	button->isPressed = false;
	button->hasMouse = false;
	button->image = NULL;
	button->imagePressed = NULL;
	
	button->caption = string_new_string(caption);
	button_adjustSize(button);
	
	// Fill listener list
	widget_addMouseListener(&button->widget, ME_PRESSED, button_mousePressed);
	widget_addMouseListener(&button->widget, ME_RELEASED, button_mouseReleased);	
	widget_addMouseListener(&button->widget, ME_ENTERED, button_mouseEntered);	
	widget_addMouseListener(&button->widget, ME_LEFT, button_mouseLeft);
}

void button_draw(const guiButton_t* button, guiGraphics_t* graphics)
{
	guiDebugPrint("drawing button");
	graph_drawText(graphics, 0,	0, button_getCaption(button)->s);
}

void button_setCaption(guiButton_t* button, const string_t* caption)
{
	string_copy(button->caption, caption, 0, string_size(caption));
}

const string_t* button_getCaption(const guiButton_t* button)
{
	return button->caption;
}

void button_adjustSize(guiButton_t* button)
{
	if (button->image) {
		widget_setSize((guiWidget_t*)button, image_getWidth(*button->image), image_getHeight(*button->image));
	} else {
		widget_setSize((guiWidget_t*)button,
			font_getWidthString(widget_getFont(button), button_getCaption(button)) + 8, font_getCharHeight(widget_getFont(button)) + 8);
	}
}

void button_mousePressed(void *widget, mouseEvent_t* mouseEvent)
{
	guiDebugPrint("button pressed");
	guiButton_t *button = (guiButton_t *)widget;
	if (mouseEvent->button == LEFT) {
		button->isPressed = true;
	}
}

void button_mouseReleased(void *widget, mouseEvent_t* mouseEvent)
{
	guiDebugPrint("button released");
	guiButton_t *button = (guiButton_t *)widget;
	if (mouseEvent->button == LEFT) {
		if (button->isPressed && button->hasMouse) {
			button->isPressed = false;
			mouseEvent_t *event = new(mouseEvent_t);
			*event = *mouseEvent;
			widget_handleEvent((guiWidget_t*)widget, (event_t*)event);
		} else {
			button->isPressed = false;
		}
	}
}

void button_mouseEntered(void *widget, mouseEvent_t* mouseEvent)
{
	guiButton_t *button = (guiButton_t *)widget;
	button->hasMouse = true;
}

void button_mouseLeft(void *widget, mouseEvent_t* mouseEvent)
{
	guiButton_t *button = (guiButton_t *)widget;
	button->hasMouse = false;
}
