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
	void (*w_destructor)(struct guiWidget_s *widget);
	struct guiRectangle_s *(*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s *(*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void(*w_draw)(const struct guiCheckBox *container, guiGraphics_t *graphics);
	void(*w_tick)(struct guiWidget_s *widget);
	bool(*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
	void(*w_setFocusHandler)(struct guiWidget_s *widget, void *focus);
} guiCheckBox_vf_t;

typedef struct guiCheckBox {
	guiWidget_t widget;
	string_t *caption;
	bool isChecked;
	bool mouseDown;

	guiImage_t *image;
	guiImage_t *imagePressed;
	guiImage_t *imageDisabled;
	guiImage_t *imageChecked;
	guiImage_t *imageCheckedPressed;
	guiImage_t *imageCheckedDisabled;
} guiCheckBox;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

const char *checkbox_typename(guiCheckBox *widget);
guiCheckBox *checkbox_new(guiGUI_t *gui, const string_t *caption);
void checkbox_init(guiCheckBox *checkbox, const string_t *caption);

void checkbox_draw(const guiCheckBox *checkbox, guiGraphics_t *graphics);

#define checkbox_isChecked(_widget) (((guiCheckBox*)_widget)->isChecked)
#define checkbox_setChecked(_widget, _state) { ((guiCheckBox*)_widget)->isChecked = _state; }

#define checkbox_getCaption(_widget) (((guiCheckBox*)_widget)->caption)
#define checkbox_setCaption(_widget, _caption) { ((guiCheckBox*)_widget)->caption = _caption; }

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
void checkbox_mousePressed(void *widget, mouseEvent_t *mouseEvent);
void checkbox_mouseReleased(void *widget, mouseEvent_t *mouseEvent);
void checkbox_mouseClicked(void *widget, mouseEvent_t *mouseEvent);

#endif // WIDGETS_CHECKBOX_H_INCLUDED
