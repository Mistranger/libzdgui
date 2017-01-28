#ifndef WIDGETS_BUTTON_H_INCLUDED
#define WIDGETS_BUTTON_H_INCLUDED

#include "event.h"
#include "event/event_focus.h"
#include "event/event_mouse.h"
#include "gui.h"
#include "image.h"
#include "util/string.h"
#include "widget.h"

struct guiButton;

extern const char *ButtonType;

typedef struct guiButton_vf {
	const char *(*w_typename)(struct guiButton *widget);
	void (*w_destructor)(struct guiButton *widget);
	struct guiRectangle_s *(*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s *(*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void (*w_draw)(const struct guiButton *button, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
	void(*w_setFocusHandler)(struct guiWidget_s *widget, void *focus);
} guiButton_vf_t;

typedef struct guiButton {
	guiWidget_t widget;

	bool isPressed;
	bool hasMouse;

	string_t *caption;
	guiImage_t *image;
	guiImage_t *imageDisabled;
	guiImage_t *imagePressed;
	guiImage_t *imageHover;
} guiButton;

// Virtual inherited from guiWidget_t
const char *button_typename(guiButton *widget);
void button_draw(const guiButton *button, guiGraphics_t *graphics);
//guiRectangle_t* button_getChildrenArea(const guiWidget_t *widget);
//guiWidget_t* button_getWidgetAt(const guiWidget_t *widget, int x, int y);
//void button_tick(guiWidget_t *widget);
//bool button_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist);

// Constructor
guiButton *button_new(guiGUI_t *gui, const string_t *caption);
void button_destructor(guiButton *button);

void button_init(guiButton *button, const string_t *caption);
void button_setCaption(guiButton *button, const string_t *caption);
const string_t *button_getCaption(const guiButton *button);
void button_adjustSize(guiButton *button);

#define button_isPressed(_widget) (((guiButton*)_widget)->isPressed)
#define button_setPressed(_widget, _state) { ((guiButton*)_widget)->isPressed = _state; }
#define button_hasMouse(_widget) (((guiButton*)_widget)->hasMouse)

#define button_getImage(_widget) (((guiButton*)_widget)->image)
#define button_setImage(_widget, _image) { ((guiButton*)_widget)->image = _image; }
#define button_getImagePressed(_widget) (((guiButton*)_widget)->imagePressed)
#define button_setImagePressed(_widget, _image) { ((guiButton*)_widget)->imagePressed = _image; }
#define button_getImageDisabled(_widget) (((guiButton*)_widget)->imageDisabled)
#define button_setImageDisabled(_widget, _image) { ((guiButton*)_widget)->imageDisabled = _image; }
#define button_getImageHover(_widget) (((guiButton*)_widget)->imageHover)
#define button_setImageHover(_widget, _image) { ((guiButton*)_widget)->imageHover = _image; }

// Event listeners
void button_mousePressed(void *widget, mouseEvent_t *mouseEvent);
void button_mouseReleased(void *widget, mouseEvent_t *mouseEvent);
void button_mouseEntered(void *widget, mouseEvent_t *mouseEvent);
void button_mouseLeft(void *widget, mouseEvent_t *mouseEvent);
void button_focusLost(void *widget, focusEvent_t *focusEvent);
#endif // WIDGETS_BUTTON_H_INCLUDED
