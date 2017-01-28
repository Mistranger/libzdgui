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
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void (*w_draw)(const struct guiButton *button, guiGraphics *graphics);
	void (*w_tick)(struct guiWidget *widget);
	bool (*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiWidget *widget, void *focus);
} guiButton_vf_t;

typedef struct guiButton {
	guiWidget widget;

	bool isPressed;
	bool hasMouse;

	string_t *caption;
	guiImage *image;
	guiImage *imageDisabled;
	guiImage *imagePressed;
	guiImage *imageHover;
} guiButton;

// Virtual inherited from guiWidget
const char *button_typename(guiButton *widget);
void button_draw(const guiButton *button, guiGraphics *graphics);

// Constructor
guiButton *button_new(guiGUI *gui, const string_t *caption);
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
void button_mousePressed(void *widget, guiMouseEvent *mouseEvent);
void button_mouseReleased(void *widget, guiMouseEvent *mouseEvent);
void button_mouseEntered(void *widget, guiMouseEvent *mouseEvent);
void button_mouseLeft(void *widget, guiMouseEvent *mouseEvent);
void button_focusLost(void *widget, guiFocusEvent *focusEvent);
#endif // WIDGETS_BUTTON_H_INCLUDED
