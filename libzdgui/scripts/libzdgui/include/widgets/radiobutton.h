#ifndef WIDGETS_RADIOBUTTON_H_INCLUDED
#define WIDGETS_RADIOBUTTON_H_INCLUDED

#include "event.h"
#include "event/event_mouse.h"
#include "gui.h"
#include "image.h"
#include "util/string.h"
#include "widget.h"

struct guiRadioButton;

extern const char *RadioButtonType;

typedef struct guiRadioButton_vf {
	const char *(*w_typename)(struct guiRadioButton *widget);
	void(*w_destructor)(struct guiRadioButton *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void(*w_draw)(const struct guiRadioButton *radiobutton, guiGraphics *graphics);
	void(*w_tick)(struct guiWidget *widget);
	bool(*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiWidget *widget, void *focus);
} guiRadioButton_vf_t;

typedef struct guiRadioButton {
	guiWidget widget;

	bool checked;
	bool mouseDown;
	string_t *caption;

	guiImage *image;
	guiImage *imagePressed;
	guiImage *imageDisabled;
	guiImage *imageChecked;
	guiImage *imageCheckedPressed;
	guiImage *imageCheckedDisabled;
} guiRadioButton;

// Virtual inherited from guiWidget
void radio_draw(const guiRadioButton *radiobutton, guiGraphics *graphics);

const char *radio_typename(guiRadioButton *widget);
// Constructor
guiRadioButton *radio_new(guiGUI *gui, const string_t *caption);
void radio_destructor(guiRadioButton *radiobutton);

void radio_init(guiRadioButton *radiobutton, const string_t *caption);
void radio_setCaption(guiRadioButton *radiobutton, const string_t *caption);
const string_t *radio_getCaption(const guiRadioButton *radiobutton);
void radio_adjustSize(guiRadioButton *radiobutton);

#define radio_isChecked(_widget) (((guiRadioButton*)_widget)->checked)
void radio_setChecked(guiRadioButton *radiobutton, bool state);
#define radio_isMouseDown(_widget) (((guiRadioButton*)_widget)->mouseDown)
#define radio_setMouseDown(_widget, _state) { ((guiRadioButton*)_widget)->mouseDown = _state; }

#define radio_getImage(_widget) (((guiRadioButton*)_widget)->image)
#define radio_setImage(_widget, _image) { ((guiRadioButton*)_widget)->image = _image; }
#define radio_getImagePressed(_widget) (((guiRadioButton*)_widget)->imagePressed)
#define radio_setImagePressed(_widget, _image) { ((guiRadioButton*)_widget)->imagePressed = _image; }
#define radio_getImageDisabled(_widget) (((guiRadioButton*)_widget)->imageDisabled)
#define radio_setImageDisabled(_widget, _image) { ((guiRadioButton*)_widget)->imageDisabled = _image; }
#define radio_getImageChecked(_widget) (((guiRadioButton*)_widget)->image)
#define radio_setImageChecked(_widget, _image) { ((guiRadioButton*)_widget)->image = _image; }
#define radio_getImageCheckedPressed(_widget) (((guiRadioButton*)_widget)->imagePressed)
#define radio_setImageCheckedPressed(_widget, _image) { ((guiRadioButton*)_widget)->imagePressed = _image; }
#define radio_getImageCheckedDisabled(_widget) (((guiRadioButton*)_widget)->imageDisabled)
#define radio_setImageCheckedDisabled(_widget, _image) { ((guiRadioButton*)_widget)->imageDisabled = _image; }

// Event listeners
void radiobutton_mousePressed(void *widget, guiMouseEvent *mouseEvent);
void radiobutton_mouseReleased(void *widget, guiMouseEvent *mouseEvent);
void radiobutton_mouseClicked(void *widget, guiMouseEvent *mouseEvent);

#endif // WIDGETS_RADIOBUTTON_H_INCLUDED
