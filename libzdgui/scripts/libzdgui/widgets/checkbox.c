#include "system.h"
#include "widgets/checkbox.h"

#include "event/event_widget.h"
#include "util/util.h"

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

const guiImage cboxDefImage = {15, 15, s"CHECKOFF"};
const guiImage cboxDefImageChecked = {15, 15, s"CHECKON"};
const guiImage cboxDefImagePressed = {16, 16, s"CHECKPRS"};
const guiImage cboxDefImageDisabled = {15, 15, s"CBOFFDIS"};
const guiImage cboxDefImageCheckedDisabled = {15, 15, s"CBONDIS"};

const char *CheckBoxType = "CheckBox";

const char *checkbox_typename(guiCheckBox *widget)
{
	return CheckBoxType;
}

guiCheckBox *checkbox_new(guiGUI *gui, const string_t *caption)
{
	guiCheckBox *checkbox = new (guiCheckBox);
	checkbox_init(checkbox, caption);
	gui_addWidget(gui, checkbox);
	return checkbox;
}

void checkbox_init(guiCheckBox *checkbox, const string_t *caption)
{
	widget_init(&checkbox->widget);

	checkbox->widget.v = (guiWidget_vf_t *)&guiCheckBox_vtable;
	
	checkbox->isChecked = false;
	checkbox->imageChecked = (guiImage*)&cboxDefImageChecked;
	checkbox->imageCheckedDisabled = (guiImage*)&cboxDefImageCheckedDisabled;
	checkbox->imageCheckedPressed = (guiImage*)&cboxDefImagePressed;
	checkbox->image = (guiImage*)&cboxDefImage;
	checkbox->imageDisabled = (guiImage*)&cboxDefImageDisabled;
	checkbox->imagePressed = (guiImage*)&cboxDefImagePressed;
	checkbox_setCaption(checkbox, caption);

	widget_addMouseListener(&checkbox->widget, ME_PRESSED, checkbox_mousePressed);
	widget_addMouseListener(&checkbox->widget, ME_RELEASED, checkbox_mouseReleased);
	widget_addMouseListener(&checkbox->widget, ME_CLICKED, checkbox_mouseClicked);
}

void checkbox_draw(const guiCheckBox *checkbox, guiGraphics *graphics)
{
	guiImage *img = NULL;
	if (checkbox_isChecked(checkbox)) {
		if (!widget_isEnabled(checkbox)) {
			img = checkbox->imageCheckedDisabled;
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

	graph_drawText(graphics, widget_getFont(checkbox), width - 2, 
		abs(image_getHeight(*checkbox->image) - font_getCharHeight(*widget_getFont(checkbox))) / 2,
		widget_getFontColor(checkbox), string_cstr(checkbox_getCaption(checkbox)));
}

void checkbox_adjustSize(guiCheckBox *checkbox)
{
	int width, height;

	height = font_getCharHeight(*widget_getFont(checkbox));
	if (checkbox->image) {
		width = image_getWidth(*checkbox->image);
		width += width / 2;
		height = MAX(height, image_getHeight(*checkbox->image));
	} else {
		width = font_getCharHeight(*widget_getFont(checkbox));
		width += width / 2;
	}

	widget_setSize(checkbox, font_getWidthString(widget_getFont(checkbox), checkbox_getCaption(checkbox), false) + width, height);
}


void checkbox_mousePressed(void *widget, guiMouseEvent *mouseEvent)
{
	guiCheckBox *checkbox = (guiCheckBox *)widget;
	if (mouseEvent->button == MB_LEFT) {
		checkbox->mouseDown = true;
	}
}

void checkbox_mouseReleased(void *widget, guiMouseEvent *mouseEvent)
{
	guiCheckBox *checkbox = (guiCheckBox *)widget;
	if (mouseEvent->button == MB_LEFT) {
		checkbox->mouseDown = false;
	}
}

void checkbox_mouseClicked(void *widget, guiMouseEvent *mouseEvent)
{
	guiCheckBox *checkbox = (guiCheckBox *)widget;
	if (mouseEvent->button == MB_LEFT) {
		checkbox_setChecked(checkbox, !checkbox_isChecked(checkbox));
		guiWidgetEvent *changed = widgetEvent_new(checkbox, WE_VALUE_CHANGED);
		widget_handleEvent((guiWidget *)checkbox, (guiEvent *)changed, true);
	}
}
