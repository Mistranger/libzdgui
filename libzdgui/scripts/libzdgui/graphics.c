#include "system.h"
#include "graphics.h"

void graph_init(guiGraphics_t* graphics, int width, int height)
{
	graphics->screenWidth = width;
	graphics->screenHeight = height;
	graphics->clipStack = stack_init();
	ACS_SetHudSize(width, height, 1); 
}

void graph_pushClipArea(guiGraphics_t *graphics, guiRectangle_t area)
{
	if (!stack_size(graphics->clipStack)) {
		guiClipRectangle_t *clip = (guiClipRectangle_t*)malloc(sizeof(guiClipRectangle_t));
		clip->rect.x = area.x;
		clip->rect.y = area.y;
		clip->rect.width = area.width;
		clip->rect.height = area.height;
		stack_push(graphics->clipStack, clip);
	}
	guiClipRectangle_t *clip = (guiClipRectangle_t*)malloc(sizeof(guiClipRectangle_t));
	guiClipRectangle_t *top = (guiClipRectangle_t*)graphics->clipStack->head->data;
	*clip = *((guiClipRectangle_t*)&area);
	clip->xOffset = top->xOffset + clip->rect.x;
	clip->yOffset = top->yOffset + clip->rect.y;
	clip->rect.x += top->xOffset;
	clip->rect.y += top->yOffset;
	
	if (clip->rect.x < top->rect.x) {
		clip->rect.x = top->rect.x; 
	}
        
	if (clip->rect.y < top->rect.y) {
		clip->rect.y = top->rect.y;            
	}
			
	if (clip->rect.width > top->rect.width) {
		clip->rect.width = top->rect.width;                
	}
	
	if (clip->rect.height > top->rect.height) {
		clip->rect.height = top->rect.height;             
	}
	stack_push(graphics->clipStack, clip);
	
	ACS_SetHudClipRect(clip->rect.x, clip->rect.y, clip->rect.width, clip->rect.height);

}

void graph_popClipArea(guiGraphics_t *graphics)
{
	if (!graphics->clipStack) {
		
	}
	stack_pop(graphics->clipStack);
	guiRectangle_t *top = (guiRectangle_t*)graphics->clipStack->head->data;
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
	
	// Get actual drawing coordinates
	int dx = x, dy = y;
	guiClipRectangle_t *top = (guiClipRectangle_t*)graphics->clipStack->head->data;
	dx += top->xOffset;
	dy += top->yOffset;
	guiDebugPrint("drawing text at %d, %d" _C_ dx _C_ dy);
	ACS_HudMessage(HUDMSG_PLAIN, 0, CR_BLACK, (fixed)dx + 0.1, (fixed)dy + 0.1, 0.03k, 0.0, 0.0, 1.0, text); 
}

int graph_getScreenHeight(guiGraphics_t *graphics)
{
	return graphics->screenWidth;
}

int graph_getScreenWidth(guiGraphics_t *graphics)
{
	return graphics->screenHeight;
}
