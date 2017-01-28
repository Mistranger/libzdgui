#include "system.h"

#include "widgets/radiobutton.h"

#include "event.h"
#include "fonts.h"
#include "graphics.h"
#include "event/event_widget.h"
#include "widgets/container.h"

const guiImage radioDefImage = {15, 15, s"RADIOOFF"};
const guiImage radioDefImageChecked = {15, 15, s"RADIOON"};
const guiImage radioDefImagePressed = {16, 16, s"RADIOSEL"};
const guiImage radioDefImageDisabled = {15, 15, s"RADIODOF"};
const guiImage radioDefImageCheckedDisabled = {15, 15, s"RADIODON"};

guiRadioButton_vf_t guiRadioButton_vtable = {
	radio_typename,
	radio_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	radio_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager
};

const char *RadioButtonType = "RadioButton";

const char *radio_typename(guiRadioButton *widget)
{
	return RadioButtonType;
}

guiRadioButton *radio_new(guiGUI *gui, const string_t *caption)
{
	guiRadioButton *radiobutton = new (guiRadioButton);
	radio_init(radiobutton, caption);
	gui_addWidget(gui, radiobutton);
	return radiobutton;
}

void radio_destructor(guiRadioButton *radiobutton)
{
	string_delete(radiobutton->caption);
	widget_destructor((guiWidget *)radiobutton);
}

void radio_init(guiRadioButton *radiobutton, const string_t *caption)
{
	widget_init(&radiobutton->widget);

	radiobutton->widget.v = (guiWidget_vf_t *)&guiRadioButton_vtable;

	widget_setFont(radiobutton, (guiFont *)&FONT_CONFONT);
	widget_setFocusable(radiobutton, true);
	radiobutton->mouseDown = false;
	radiobutton->checked = false;
	radiobutton->imageChecked = (guiImage*)&radioDefImageChecked;
	radiobutton->imageCheckedDisabled = (guiImage*)&radioDefImageCheckedDisabled;
	radiobutton->imageCheckedPressed = (guiImage*)&radioDefImagePressed;
	radiobutton->image = (guiImage*)&radioDefImage;
	radiobutton->imageDisabled = (guiImage*)&radioDefImageDisabled;
	radiobutton->imagePressed = (guiImage*)&radioDefImagePressed;

	radiobutton->caption = string_new_string(caption);
	radio_adjustSize(radiobutton);

	// Fill listener list
	widget_addMouseListener(&radiobutton->widget, ME_PRESSED, radiobutton_mousePressed);
	widget_addMouseListener(&radiobutton->widget, ME_CLICKED, radiobutton_mouseClicked);
	widget_addMouseListener(&radiobutton->widget, ME_RELEASED, radiobutton_mouseReleased);
}

void radio_draw(const guiRadioButton *radiobutton, guiGraphics *graphics)
{
	guiDebugPrint("drawing radiobutton");
	guiImage *img = NULL;
	if (radio_isChecked(radiobutton)) {
		if (!widget_isEnabled(radiobutton)) {
			img = radiobutton->imageCheckedDisabled;
		} else if (radiobutton->mouseDown) {
			img = radiobutton->imageCheckedPressed;
		} else {
			img = radiobutton->imageChecked;
		}
	} else {
		if (!widget_isEnabled(radiobutton)) {
			img = radiobutton->imageDisabled;
		} else if (radiobutton->mouseDown) {
			img = radiobutton->imagePressed;
		} else {
			img = radiobutton->image;
		}
	}

	if (img) {
		graph_drawImage(graphics, 0, (widget_getHeight(radiobutton) - image_getHeight(*img)), image_getImage(*img));
	}

	int width;
	if (radiobutton->image) {
		width = image_getWidth(*radiobutton->image);
		width += width / 2;
	} else {
		width = widget_getHeight(radiobutton);
		width += width / 2;
	}

	graph_drawText(graphics, widget_getFont(radiobutton), width - 2, 0, widget_getFontColor(radiobutton), string_cstr(radio_getCaption(radiobutton)));
}

void radio_setCaption(guiRadioButton *radiobutton, const string_t *caption)
{
	string_copy(radiobutton->caption, caption, 0, string_size(caption));
}

const string_t *radio_getCaption(const guiRadioButton *radiobutton)
{
	return radiobutton->caption;
}

void radio_adjustSize(guiRadioButton *radiobutton)
{
	int width, height;

	height = font_getCharHeight(*widget_getFont(radiobutton));
	if (radiobutton->image) {
		width = image_getWidth(*radiobutton->image);
		width += width / 2;
		height = MAX(height, image_getHeight(*radiobutton->image));
	} else {
		width = font_getCharHeight(*widget_getFont(radiobutton));
		width += width / 2;
	}

	widget_setSize(radiobutton, font_getWidthString(widget_getFont(radiobutton), radio_getCaption(radiobutton), false) + width, height);
}

void radio_setChecked(guiRadioButton *radiobutton, bool state)
{
	if (state) {
		guiWidget *parent = widget_getParent(radiobutton);
		if (!parent || !parent->isContainer) {
			guiWarning("Only one option for this radiobutton");
			return;
		}
		guiContainer *c = (guiContainer *)parent;
		guiWidget *w;
		for (listNode_t *node = c->children->head; node; node = node->next) {
			w = (guiWidget *)node->data;
			if (!strcmp(w->v->w_typename(w), RadioButtonType)) {
				radio_setChecked((guiRadioButton *)w, false);
			}
		}

	}
	radiobutton->checked = state;
}

void radiobutton_mousePressed(void *widget, guiMouseEvent *mouseEvent)
{
	guiDebugPrint("radiobutton pressed");
	guiRadioButton *radiobutton = (guiRadioButton *)widget;
	if (mouseEvent->button == MB_LEFT) {
		radiobutton->mouseDown = true;
	}
}

void radiobutton_mouseReleased(void *widget, guiMouseEvent *mouseEvent)
{
	guiDebugPrint("radiobutton released");
	guiRadioButton *radiobutton = (guiRadioButton *)widget;
	if (mouseEvent->button == MB_LEFT) {
		radiobutton->mouseDown = false;
	}
}

void radiobutton_mouseClicked(void *widget, guiMouseEvent *mouseEvent)
{
	guiDebugPrint("radiobutton clicked");
	guiRadioButton *radiobutton = (guiRadioButton *)widget;
	if (mouseEvent->button == MB_LEFT) {
		if (!radiobutton->checked) {
			radio_setChecked(radiobutton, true);
			guiWidgetEvent *changed = widgetEvent_new(radiobutton, WE_VALUE_CHANGED);
			widget_handleEvent((guiWidget*)radiobutton, (guiEvent*)changed, true);
		}
	}
}
