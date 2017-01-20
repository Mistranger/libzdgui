#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "widget.h"

const int MAX_CAPTION_LENGTH = 256;

enum ButtonFlags
{
	BF_PRESSED = 0x00000001
};

typedef struct guiButton_s
{
    guiWidget_t *widget;
	int buttonFlags;
	char caption[MAX_CAPTION_LENGTH];
	str image;
} guiButton_t;

// Virtual inherited from guiWidget_t
void button_draw(const guiWidget_t *widget, guiGraphics_t *graphics);


void button_init(guiButton_t *button);
void button_setCaption(guiButton_t *button, const char *caption);
const char* button_getCaption(const guiButton_t *button);

#endif // BUTTON_H_INCLUDED
