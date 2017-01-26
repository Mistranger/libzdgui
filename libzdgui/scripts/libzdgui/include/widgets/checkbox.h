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

struct guiCheckBox_s;

typedef struct guiCheckBox_vf {
	const char* (*w_typename)(struct guiCheckBox_s *widget);
	void (*w_destructor)(struct guiWidget_s *widget);
	struct guiRectangle_s* (*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void(*w_draw)(const struct guiCheckBox_s *container, guiGraphics_t *graphics);
	void(*w_tick)(struct guiWidget_s *widget);
	bool(*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
} guiCheckBox_vf_t;

typedef struct guiCheckBox_s
{
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
} guiCheckBox_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

const char* checkbox_typename(guiCheckBox_t *widget);
guiCheckBox_t* checkbox_new(guiGUI_t *gui, const string_t *caption);
void checkbox_init(guiCheckBox_t *checkbox, const string_t *caption);

void checkbox_draw(const guiCheckBox_t *checkbox, guiGraphics_t *graphics);

#define checkbox_isChecked(_widget) (((guiCheckBox_t*)_widget)->isChecked)
#define checkbox_setChecked(_widget, _state) { ((guiCheckBox_t*)_widget)->isChecked = _state; }

#define checkbox_getCaption(_widget) (((guiCheckBox_t*)_widget)->caption)
#define checkbox_setCaption(_widget, _caption) { ((guiCheckBox_t*)_widget)->caption = _caption; }

#define checkbox_getImage(_widget) (((guiCheckBox_t*)_widget)->image)
#define checkbox_setImage(_widget, _image) { ((guiCheckBox_t*)_widget)->image = _image; }
#define checkbox_getImagePressed(_widget) (((guiCheckBox_t*)_widget)->imagePressed)
#define checkbox_setImagePressed(_widget, _image) { ((guiCheckBox_t*)_widget)->imagePressed = _image; }
#define checkbox_getImageDisabled(_widget) (((guiCheckBox_t*)_widget)->imageDisabled)
#define checkbox_setImageDisabled(_widget, _image) { ((guiCheckBox_t*)_widget)->imageDisabled = _image; }
#define checkbox_getImageChecked(_widget) (((guiCheckBox_t*)_widget)->image)
#define checkbox_setImageChecked(_widget, _image) { ((guiCheckBox_t*)_widget)->image = _image; }
#define checkbox_getImageCheckedPressed(_widget) (((guiCheckBox_t*)_widget)->imagePressed)
#define checkbox_setImageCheckedPressed(_widget, _image) { ((guiCheckBox_t*)_widget)->imagePressed = _image; }
#define checkbox_getImageCheckedDisabled(_widget) (((guiCheckBox_t*)_widget)->imageDisabled)
#define checkbox_setImageCheckedDisabled(_widget, _image) { ((guiCheckBox_t*)_widget)->imageDisabled = _image; }

// Event listeners
void checkbox_mousePressed(void *widget, mouseEvent_t *mouseEvent);
void checkbox_mouseReleased(void *widget, mouseEvent_t *mouseEvent);
void checkbox_mouseClicked(void *widget, mouseEvent_t *mouseEvent);

#endif // WIDGETS_CHECKBOX_H_INCLUDED
