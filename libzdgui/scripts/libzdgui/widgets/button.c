#include "system.h"

#include "widgets/button.h"

#include "event.h"
#include "event/event_focus.h"
#include "fonts.h"
#include "graphics.h"

guiButton_vf_t guiButton_vtable = {
	button_typename,
	button_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	button_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager,
	widget_getMinimalSize
};

const guiImage buttonDefImage = {80, 15, s"B1"};
const guiImage buttonDefImagePressed = {80, 15, s"B1PRESS"};
const guiImage buttonDefImageHover = {80, 15, s"B1HOVER"};
const guiImage buttonDefImageDisabled = {80, 15, s"B1DIS"};

const char *ButtonType = "Button";

const char *button_typename(guiButton *widget)
{
	return ButtonType;
}

guiButton *button_new(guiGUI *gui, const string_t *caption)
{
	guiButton *button = new (guiButton);
	button_init(button, caption);
	gui_addWidget(gui, button);
	return button;
}

void button_destructor(guiButton *button)
{
	string_delete(button->caption);
	widget_destructor((guiWidget *)button);
}

void button_init(guiButton *button, const string_t *caption)
{
	widget_init(&button->widget);

	button->widget.v = (guiWidget_vf_t *)&guiButton_vtable;

	widget_setFont(button, (guiFont *)&FONT_CONFONT);
	widget_setFocusable(button, true);
	button->isPressed = false;
	button->hasMouse = false;
	button->textAlign = ALIGN_LEFT;
	button->image = (guiImage*)&buttonDefImage;
	button->imageDisabled = (guiImage*)&buttonDefImageDisabled;
	button->imagePressed = (guiImage*)&buttonDefImagePressed;
	button->imageHover = (guiImage*)&buttonDefImageHover;

	button->caption = string_new_string(caption);
	button_adjustSize(button);

	// Fill listener list
	widget_addMouseListener(&button->widget, ME_PRESSED, button_mousePressed);
	widget_addMouseListener(&button->widget, ME_RELEASED, button_mouseReleased);
	widget_addMouseListener(&button->widget, ME_ENTERED, button_mouseEntered);
	widget_addMouseListener(&button->widget, ME_LEFT, button_mouseLeft);
	guiFocusListener *focus = focusListener_new(button, FE_FOCUSLOST, button_focusLost);
	widget_addListener(button, focus);

}

void button_draw(const guiButton *button, guiGraphics *graphics)
{
	guiImage *img = button->image;
	if (!widget_isEnabled(button)) {
		img = button->imageDisabled ? button->imageDisabled : button->image;
	} else if (button_isPressed(button)) {
		img = button->imagePressed ? button->imagePressed : button->image;
	} else if (button_hasMouse(button)) {
		img = button->imageHover ? button->imageHover : button->image;
	}
	if (img) {
		graph_drawImage(graphics, 0, 0, image_getImage(*img));
	}
	
	int textX;
	int textY = widget_getHeight(button) / 2 - font_getCharHeight(*widget_getFont(button)) / 2;

	switch (button_getAlignment(button)) {
		case ALIGN_LEFT:
			textX = 4;
			break;
		case ALIGN_CENTER:
			textX = widget_getWidth(button) / 2;
			break;
		case ALIGN_RIGHT:
			textX = widget_getWidth(button) - 4;
			break;
		default:
			textX = 0;
			guiWarning("Unknown alignment.");
	}
	textX +=  button_isPressed(button) ? 1 : 0;
	textY +=  button_isPressed(button) ? 1 : 0;
	
	graph_drawText(graphics, widget_getFont(button), textX, textY,
		widget_getFontColor(button), button_getCaption(button)->s);
}

void button_setCaption(guiButton *button, const string_t *caption)
{
	string_copy(button->caption, caption, 0, string_size(caption));
}

const string_t *button_getCaption(const guiButton *button)
{
	return button->caption;
}

void button_adjustSize(guiButton *button)
{
	if (button->image) {
		widget_setSize((guiWidget *)button, image_getWidth(*button->image), image_getHeight(*button->image));
	} else {
		widget_setSize((guiWidget *)button,
					   font_getWidthString(widget_getFont(button), button_getCaption(button), false) + 8, font_getCharHeight(*widget_getFont(button)) + 8);
	}
}

void button_mousePressed(void *widget, guiMouseEvent *mouseEvent)
{
	guiButton *button = (guiButton *)widget;

	if (mouseEvent->button == MB_LEFT) {
		guiInfo("button pressed %d" _C_ mouseEvent->button);
		button->isPressed = true;
	}
}

void button_mouseReleased(void *widget, guiMouseEvent *mouseEvent)
{
	guiButton *button = (guiButton *)widget;
	if (mouseEvent->button == MB_LEFT) {
		guiInfo("button released");
		if (button->isPressed && button->hasMouse) {
			button->isPressed = false;
		} else {
			button->isPressed = false;
		}
	}
}

void button_mouseEntered(void *widget, guiMouseEvent *mouseEvent)
{
	guiButton *button = (guiButton *)widget;
	button->hasMouse = true;
}

void button_mouseLeft(void *widget, guiMouseEvent *mouseEvent)
{
	guiButton *button = (guiButton *)widget;
	button->hasMouse = false;
}

void button_focusLost(void *widget, guiFocusEvent *focusEvent)
{
	guiButton *button = (guiButton *)widget;
	button->isPressed = false;
}
