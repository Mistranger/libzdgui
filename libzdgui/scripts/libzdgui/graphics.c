#include "libzdgui.h"
#include "graphics.h"

void graph_init(guiGraphics_t* graphics, int width, int height)
{
	graphics->screenWidth = width;
	graphics->screenHeight = height;
	ACS_SetHudSize(width, height, 1); 
}

void graph_pushClipArea(guiGraphics_t *graphics, guiRectangle_t area)
{
	if (!graphics->clipStack) {
		guiRectangle_t *clip = (guiRectangle_t*)malloc(sizeof(guiRectangle_t));
		clip->x = area.x;
		clip->y = area.y;
		clip->width = area.width;
		clip->height = area.height;
		stack_push(graphics->clipStack, clip);
	}
	guiRectangle_t *clip = (guiRectangle_t*)malloc(sizeof(guiRectangle_t));
	guiRectangle_t *top = (guiRectangle_t*)graphics->clipStack->data;
	*clip = area;
	if (clip->x < top->x) {
		clip->x = top->x; 
	}
        
	if (clip->y < top->y) {
		clip->y = top->y;            
	}
			
	if (clip->width > top->width) {
		clip->width = top->width;                
	}
	
	if (clip->height > top->height) {
		clip->height = top->height;             
	}
	stack_push(graphics->clipStack, clip);
	
	ACS_SetHudClipRect(clip->x, clip->y, clip->width, clip->height);

}

void graph_popClipArea(guiGraphics_t *graphics)
{
	if (!graphics->clipStack) {
		
	}
	stack_pop(graphics->clipStack);
	guiRectangle_t *top = (guiRectangle_t*)graphics->clipStack->data;
	ACS_SetHudClipRect(top->x, top->y, top->width, top->height);
}

void graph_setFont(guiGraphics_t *graphics, str font)
{
	graphics->fontName = font;
	ACS_SetFont(font); 
}

void graph_drawImage(guiGraphics_t* graphics, int x, int y, str image)
{
	#pragma fixed on
	ACS_SetFont(image);
	ACS_HudMessage(HUDMSG_PLAIN, 0, CR_BLACK, x, y, 0.03k, 0.0, 0.0, 1.0, s"A"); 
	ACS_SetFont(graphics->fontName);
}

void graph_drawText(guiGraphics_t* graphics, int x, int y, const char* format, ...)
{
	va_list args;
	
	ACS_BeginStrParam();
    va_start (args, format);
    __vnprintf(format, args);
    va_end (args);
    str text = ACS_EndStrParam();
	ACS_HudMessage(HUDMSG_PLAIN, 0, CR_BLACK, (fixed)x + 0.1, (fixed)y + 0.1, 0.03k, 0.0, 0.0, 1.0, text); 
}

int graph_getScreenHeight(guiGraphics_t *graphics)
{
	return graphics->screenWidth;
}

int graph_getScreenWidth(guiGraphics_t *graphics)
{
	return graphics->screenHeight;
}
