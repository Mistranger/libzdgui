#include "libzdgui.h"

#include "widgets/button.h"

#include "graphics.h"

void button_init(guiButton_t* button)
{
	button->widget->guiWidget_draw = button_draw;
	button->widget->flags |= WF_FOCUSABLE;
	button->buttonFlags = 0;
}

void button_draw(const guiWidget_t* widget, guiGraphics_t* graphics)
{
	const guiButton_t *button = (const guiButton_t *)widget;
	graph_drawText(graphics, 
		widget_getX(button->widget),
		widget_getY(button->widget),
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
