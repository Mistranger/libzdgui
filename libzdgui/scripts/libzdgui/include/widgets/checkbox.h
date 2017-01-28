#ifndef WIDGETS_CHECKBOX_H_INCLUDED
#define WIDGETS_CHECKBOX_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "gui.h"
#include "image.h"
#include "widget.h"
#include "event/event_mouse.h"
#include "event/event_dimension.h"
#include "util/string.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

extern const char *CheckBoxType;

struct guiCheckBox;

typedef struct guiCheckBox_vf {
	const char *(*w_typename)(struct guiCheckBox *widget);
	void (*w_destructor)(struct guiWidget *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void(*w_draw)(const struct guiCheckBox *container, guiGraphics *graphics);
	void(*w_tick)(struct guiWidget *widget);
	bool(*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiWidget *widget, void *focus);
} guiCheckBox_vf_t;

typedef struct guiCheckBox {
	guiWidget widget;
	string_t *caption;
	bool isChecked;
	bool mouseDown;

	guiImage *image;
	guiImage *imagePressed;
	guiImage *imageDisabled;
	guiImage *imageChecked;
	guiImage *imageCheckedPressed;
	guiImage *imageCheckedDisabled;
} guiCheckBox;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

const char *checkbox_typename(guiCheckBox *widget);
guiCheckBox *checkbox_new(guiGUI *gui, const string_t *caption);
void checkbox_init(guiCheckBox *checkbox, const string_t *caption);

void checkbox_draw(const guiCheckBox *checkbox, guiGraphics *graphics);

void checkbox_adjustSize(guiCheckBox *checkbox);

#define checkbox_isChecked(_widget) (((guiCheckBox*)_widget)->isChecked)
#define checkbox_setChecked(_widget, _state) { ((guiCheckBox*)_widget)->isChecked = _state; }

#define checkbox_getCaption(_widget) (((guiCheckBox*)_widget)->caption)
#define checkbox_setCaption(_widget, _caption) { string_copy(((guiCheckBox*)_widget)->caption, _caption, 0, string_size(_caption)); }

#define checkbox_getImage(_widget) (((guiCheckBox*)_widget)->image)
#define checkbox_setImage(_widget, _image) { ((guiCheckBox*)_widget)->image = _image; }
#define checkbox_getImagePressed(_widget) (((guiCheckBox*)_widget)->imagePressed)
#define checkbox_setImagePressed(_widget, _image) { ((guiCheckBox*)_widget)->imagePressed = _image; }
#define checkbox_getImageDisabled(_widget) (((guiCheckBox*)_widget)->imageDisabled)
#define checkbox_setImageDisabled(_widget, _image) { ((guiCheckBox*)_widget)->imageDisabled = _image; }
#define checkbox_getImageChecked(_widget) (((guiCheckBox*)_widget)->image)
#define checkbox_setImageChecked(_widget, _image) { ((guiCheckBox*)_widget)->image = _image; }
#define checkbox_getImageCheckedPressed(_widget) (((guiCheckBox*)_widget)->imagePressed)
#define checkbox_setImageCheckedPressed(_widget, _image) { ((guiCheckBox*)_widget)->imagePressed = _image; }
#define checkbox_getImageCheckedDisabled(_widget) (((guiCheckBox*)_widget)->imageDisabled)
#define checkbox_setImageCheckedDisabled(_widget, _image) { ((guiCheckBox*)_widget)->imageDisabled = _image; }

// Event listeners
void checkbox_mousePressed(void *widget, guiMouseEvent *mouseEvent);
void checkbox_mouseReleased(void *widget, guiMouseEvent *mouseEvent);
void checkbox_mouseClicked(void *widget, guiMouseEvent *mouseEvent);

#endif // WIDGETS_CHECKBOX_H_INCLUDED
