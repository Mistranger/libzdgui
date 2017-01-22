#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "event.h"
#include "widget.h"

#define MAX_CAPTION_LENGTH 256

enum ButtonFlags
{
	BF_PRESSED = 0x00000001,
	BF_HASMOUSE = 0x00000002
};

struct guiButton_s;

typedef struct guiButton_vf {
	
	struct guiRectangle_s (*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, int x, int y);
	void (*w_draw)(const struct guiButton_s *button, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
} guiButton_vf_t;

typedef struct guiButton_s
{
    guiWidget_t widget;
	int buttonFlags;
	char caption[MAX_CAPTION_LENGTH];
	__str image;
} guiButton_t;

// Virtual inherited from guiWidget_t
void button_draw(const guiButton_t* button, guiGraphics_t* graphics);
//guiRectangle_t* button_getChildrenArea(const guiWidget_t *widget);
//guiWidget_t* button_getWidgetAt(const guiWidget_t *widget, int x, int y);
//void button_tick(guiWidget_t *widget);
//bool button_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist);

// Constructor
guiButton_t* button_new(const char* caption);

void button_init(guiButton_t *button);
void button_setCaption(guiButton_t *button, const char *caption);
const char* button_getCaption(const guiButton_t *button);
void button_adjustSize(guiButton_t *button);

// Event listeners
void button_mousePressed(void *widget, mouseEvent_t *mouseEvent);
void button_mouseReleased(void *widget, mouseEvent_t *mouseEvent);
void button_mouseEntered(void *widget, mouseEvent_t* mouseEvent);
void button_mouseLeft(void *widget, mouseEvent_t* mouseEvent);

#endif // BUTTON_H_INCLUDED
