#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "util/vecstack.h"
#include "dimension.h"
#include "fonts.h"

#include <ACS_ZDoom.h>

#define HUDMESSAGE_ID 0x0000FFFF

typedef struct guiGraphics {
	vecstack_t *clipStack;
	__str fontName;

	int screenWidth, screenHeight;
	int drawOrder;
} guiGraphics;

void graph_init(guiGraphics *graphics, int width, int height);
void graph_beginDraw(guiGraphics *graphics);
void graph_endDraw(guiGraphics *graphics);
void graph_pushClipArea(guiGraphics *graphics, const guiRectangle area);
void graph_popClipArea(guiGraphics *graphics);
int graph_getScreenWidth(guiGraphics *graphics);
int graph_getScreenHeight(guiGraphics *graphics);

void graph_setFont(guiGraphics *graphics, __str font);
void graph_drawRect(guiGraphics *graphics, int x, int y, int width, int height, __str color);
void graph_drawRectAlpha(guiGraphics *graphics, int x, int y, int width, int height, __str color, fixed alpha);
void graph_drawImage(guiGraphics *graphics, int x, int y, __str image);
void graph_drawImageScaled(guiGraphics *graphics, int dstX, int dstY,
						   int srcWidth, int srcHeight, int dstWidth, int dstHeight, __str image);
void graph_drawText(guiGraphics *graphics, guiFont *font, int x, int y, __str fontColor, const char *format, ...);


#endif // GRAPHICS_H_INCLUDED
