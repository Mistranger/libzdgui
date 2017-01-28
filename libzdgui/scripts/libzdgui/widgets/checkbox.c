#include "system.h"
#include "widgets/checkbox.h"

#include "event/event_widget.h"

guiCheckBox_vf_t guiCheckBox_vtable = {
	checkbox_typename,
	widget_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	checkbox_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager
};

const char *CheckBoxType = "CheckBox";

const char *checkbox_typename(guiCheckBox_t *widget)
{
	return CheckBoxType;
}

guiCheckBox_t *checkbox_new(guiGUI_t *gui, const string_t *caption)
{
	guiCheckBox_t *checkbox = new (guiCheckBox_t);
	checkbox_init(checkbox, caption);
	gui_addWidget(gui, checkbox);
	return checkbox;
}

void checkbox_init(guiCheckBox_t *checkbox, const string_t *caption)
{
	widget_init(&checkbox->widget);

	checkbox->widget.v = (guiWidget_vf_t *)&guiCheckBox_vtable;


	checkbox->imageChecked = NULL;
	checkbox->imageCheckedDisabled = NULL;
	checkbox->imageCheckedPressed = NULL;
	checkbox->image = NULL;
	checkbox->imageDisabled = NULL;
	checkbox->imagePressed = NULL;

	widget_addMouseListener(&checkbox->widget, ME_PRESSED, checkbox_mousePressed);
	widget_addMouseListener(&checkbox->widget, ME_RELEASED, checkbox_mouseReleased);
	widget_addMouseListener(&checkbox->widget, ME_CLICKED, checkbox_mouseClicked);
}

void checkbox_draw(const guiCheckBox_t *checkbox, guiGraphics_t *graphics)
{
	guiImage_t *img = NULL;
	if (checkbox_isChecked(checkbox)) {
		if (!widget_isEnabled(checkbox)) {
			img = checkbox->imageCheckedDisabled; //FIXME
		} else if (checkbox->mouseDown) {
			img = checkbox->imageCheckedPressed;
		} else {
			img = checkbox->imageChecked;
		}
	} else {
		if (!widget_isEnabled(checkbox)) {
			img = checkbox->imageDisabled;
		} else if (checkbox->mouseDown) {
			img = checkbox->imagePressed;
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
	} else {
		width = widget_getHeight(checkbox);
		width += width / 2;
	}

	graph_drawText(graphics, widget_getFont(checkbox), width - 2, 0, string_cstr(checkbox_getCaption(checkbox)));
}

void checkbox_mousePressed(void *widget, mouseEvent_t *mouseEvent)
{
	guiCheckBox_t *checkbox = (guiCheckBox_t *)widget;
	if (mouseEvent->button == MB_LEFT) {
		checkbox->mouseDown = true;
	}
}

void checkbox_mouseReleased(void *widget, mouseEvent_t *mouseEvent)
{
	guiCheckBox_t *checkbox = (guiCheckBox_t *)widget;
	if (mouseEvent->button == MB_LEFT) {
		checkbox->mouseDown = false;
	}
}

void checkbox_mouseClicked(void *widget, mouseEvent_t *mouseEvent)
{
	guiCheckBox_t *checkbox = (guiCheckBox_t *)widget;
	if (mouseEvent->button == MB_LEFT) {
		checkbox_setChecked(checkbox, !checkbox_isChecked(checkbox));
		widgetEvent_t *changed = widgetEvent_new(checkbox, WE_VALUE_CHANGED);
		widget_handleEvent((guiWidget_t *)checkbox, (event_t *)changed, true);
	}
}
