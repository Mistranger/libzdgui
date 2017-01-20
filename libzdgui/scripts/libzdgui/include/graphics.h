#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <stdbool.h>
#include "util/stack.h"
#include "dimension.h"

#include <ACS_ZDoom.h>

typedef struct guiGraphics_s
{
	stackNode_t *clipStack;
	__str fontName;
	
	int screenWidth, screenHeight;
} guiGraphics_t;

void graph_init(guiGraphics_t *graphics, int width, int height);
void graph_pushClipArea(guiGraphics_t *graphics, const guiRectangle_t area);
void graph_popClipArea(guiGraphics_t *graphics);
int graph_getScreenWidth(guiGraphics_t *graphics);
int graph_getScreenHeight(guiGraphics_t *graphics);

void graph_setFont(guiGraphics_t *graphics, __str font);
void graph_drawImage(guiGraphics_t *graphics, int x, int y, __str image);
void graph_drawText(guiGraphics_t *graphics, int x, int y, const char *format, ...);


#endif // GRAPHICS_H_INCLUDED
