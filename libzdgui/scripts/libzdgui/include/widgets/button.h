#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "event.h"
#include "widget.h"

struct guiButton_s;

typedef struct guiButton_vf {
	
	struct guiRectangle_s* (*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void (*w_draw)(const struct guiButton_s *button, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
} guiButton_vf_t;

typedef struct guiButton_s
{
    guiWidget_t widget;

	bool isPressed;
	bool hasMouse;

	string_t *caption;
	guiImage_t *image;
	guiImage_t *imagePressed;
} guiButton_t;

// Virtual inherited from guiWidget_t
void button_draw(const guiButton_t* button, guiGraphics_t* graphics);
//guiRectangle_t* button_getChildrenArea(const guiWidget_t *widget);
//guiWidget_t* button_getWidgetAt(const guiWidget_t *widget, int x, int y);
//void button_tick(guiWidget_t *widget);
//bool button_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist);

// Constructor
guiButton_t* button_new(const string_t* caption, const guiFont_t *font);

void button_init(guiButton_t* button, const string_t* caption, const guiFont_t *font);
void button_setCaption(guiButton_t *button, const string_t *caption);
const string_t* button_getCaption(const guiButton_t *button);
void button_adjustSize(guiButton_t *button);

#define button_getImage(_widget) (((guiButton_t*)_widget)->image)
#define button_setImage(_widget, _image) { ((guiButton_t*)_widget)->image = _image; }
#define button_getImagePressed(_widget) (((guiButton_t*)_widget)->imagePressed)
#define button_setImagePressed(_widget, _image) { ((guiButton_t*)_widget)->imagePressed = _image; }

// Event listeners
void button_mousePressed(void *widget, mouseEvent_t *mouseEvent);
void button_mouseReleased(void *widget, mouseEvent_t *mouseEvent);
void button_mouseEntered(void *widget, mouseEvent_t* mouseEvent);
void button_mouseLeft(void *widget, mouseEvent_t* mouseEvent);

#endif // BUTTON_H_INCLUDED
