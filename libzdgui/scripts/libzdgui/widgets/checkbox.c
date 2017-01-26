#include "system.h"
#include "widgets/checkbox.h"

guiCheckBox_vf_t guiCheckBox_vtable = {
	widget_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	checkbox_draw,
	widget_tick,
	widget_isWidgetExisting
};

guiCheckBox_t* checkbox_new(guiGUI_t *gui, const string_t *caption)
{
	guiCheckBox_t *checkbox = new(guiCheckBox_t);
	checkbox_init(checkbox, caption);
	gui_addWidget(gui, checkbox);
	return checkbox;
}

void checkbox_init(guiCheckBox_t *checkbox, const string_t *caption)
{
	widget_init(&checkbox->widget);

	checkbox->widget.v = (guiWidget_vf_t*)&guiCheckBox_vtable;

	widget_addMouseListener(&checkbox->widget, ME_PRESSED, checkbox_mousePressed);
	widget_addMouseListener(&checkbox->widget, ME_RELEASED, checkbox_mouseReleased);
	widget_addMouseListener(&checkbox->widget, ME_CLICKED, checkbox_mouseClicked);
}

void checkbox_draw(const guiCheckBox_t *checkbox, guiGraphics_t *graphics)
{
	guiImage_t *img;
	if (checkbox_isChecked(checkbox)) {
		if (!widget_isEnabled(checkbox)) {
			img = checkbox->imageChecked; //FIXME
		} else if (checkbox->mouseDown) {
			img = checkbox->imageChecked;
		} else {
			img = checkbox->imageChecked;
		}
	} else {
		if (!widget_isEnabled(checkbox)) {
			img = checkbox->image;
		} else if (checkbox->mouseDown) {
			img = checkbox->image;
		} else {
			img = checkbox->image;
		}
	}

	if (img) {
		graph_drawImage(graphics, 0, (widget_getHeight(checkbox) - image_getHeight(*img)), image_getImage(*img));
	}

	int width;
	if (checkbox->image) {
		width = image_getWidth(*checkbox->image);
		width += width / 2;
	}
	else {
		width = widget_getHeight(checkbox);
		width += width / 2;
	}

	graph_drawText(graphics, width - 2, 0, string_cstr(checkbox_getCaption(checkbox)));
}

void checkbox_mousePressed(void *widget, mouseEvent_t *mouseEvent)
{
	guiCheckBox_t *checkbox = (guiCheckBox_t*)widget;
	if (mouseEvent->button == LEFT) {
		checkbox->mouseDown = true;
	}
}

void checkbox_mouseReleased(void *widget, mouseEvent_t *mouseEvent)
{
	guiCheckBox_t *checkbox = (guiCheckBox_t*)widget;
	if (mouseEvent->button == LEFT) {
		checkbox->mouseDown = false;
	}
}

void checkbox_mouseClicked(void *widget, mouseEvent_t *mouseEvent)
{
	guiCheckBox_t *checkbox = (guiCheckBox_t*)widget;
	if (mouseEvent->button == LEFT) {
		checkbox_setChecked(checkbox, !checkbox_isChecked(checkbox));
	}
}
