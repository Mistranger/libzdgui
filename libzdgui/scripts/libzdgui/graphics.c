#include "system.h"
#include "graphics.h"

void graph_init(guiGraphics_t* graphics, int width, int height)
{
	graphics->screenWidth = width;
	graphics->screenHeight = height;
	graphics->clipStack = vecstack_init(sizeof(guiClipRectangle_t));
	graphics->drawOrder = HUDMESSAGE_ID;
	ACS_SetHudSize(width, height, 1); 
}

void graph_beginDraw(guiGraphics_t* graphics)
{
	graphics->drawOrder = HUDMESSAGE_ID;
	guiRectangle_t r = {0, 0, graph_getScreenWidth(graphics), graph_getScreenHeight(graphics)};
	graph_pushClipArea(graphics, r);
}
void graph_endDraw(guiGraphics_t* graphics)
{
	graph_popClipArea(graphics);
}

void graph_pushClipArea(guiGraphics_t *graphics, const guiRectangle_t area)
{
	if (!vecstack_size(graphics->clipStack)) {
		guiClipRectangle_t clip;
		clip.rect.pos = area.pos;
		clip.rect.width = area.width;
		clip.rect.height = area.height;
		clip.offset.x = 0;
		clip.offset.y = 0;
		guiDebugPrint("pushing (%d,%d,%d,%d) rect to clip stack, %d in stack" _C_ clip.rect.pos.x _C_ clip.rect.pos.y _C_ clip.rect.width _C_ clip.rect.height _C_ vecstack_size(graphics->clipStack));
		ACS_SetHudClipRect(clip.rect.pos.x, clip.rect.pos.y, clip.rect.width, clip.rect.height);
		vecstack_push(graphics->clipStack, &clip);
		return;
	}
	
	guiClipRectangle_t clip;
	guiClipRectangle_t top = *((guiClipRectangle_t*)vecstack_top(graphics->clipStack));
	memcpy(&clip, &area, sizeof(guiRectangle_t));
	clip.offset.x = top.offset.x + clip.rect.pos.x;
	clip.offset.y = top.offset.y + clip.rect.pos.y;
	clip.rect.pos.x += top.offset.x;
	clip.rect.pos.y += top.offset.y;
	
	if (clip.rect.pos.x < top.rect.pos.x) {
		clip.rect.pos.x = top.rect.pos.x; 
	}
        
	if (clip.rect.pos.y < top.rect.pos.y) {
		clip.rect.pos.y = top.rect.pos.y;            
	}
			
	if (clip.rect.width > top.rect.width) {
		clip.rect.width = top.rect.width;                
	}
	
	if (clip.rect.height > top.rect.height) {
		clip.rect.height = top.rect.height;             
	}
	
	rect_intersect((guiRectangle_t*)&clip, (guiRectangle_t*)&top);
	vecstack_push(graphics->clipStack, &clip);
	guiDebugPrint("pushing (%d,%d,%d,%d) rect to clip stack, %d in stack" _C_ clip.rect.pos.x _C_ clip.rect.pos.y _C_ clip.rect.width _C_ clip.rect.height _C_ vecstack_size(graphics->clipStack));
	ACS_SetHudClipRect(clip.rect.pos.x, clip.rect.pos.y, clip.rect.width, clip.rect.height);

}

void graph_popClipArea(guiGraphics_t *graphics)
{
	if (!graphics->clipStack) {
		return;
	}
	guiClipRectangle_t *clip = (guiClipRectangle_t*)vecstack_top(graphics->clipStack);
	vecstack_pop(graphics->clipStack);
	guiDebugPrint("pop (%d,%d,%d,%d) rect from clip stack, %d in stack" _C_ clip->rect.pos.x _C_ clip->rect.pos.y _C_ clip->rect.width _C_ clip->rect.height _C_ vecstack_size(graphics->clipStack));
	if (!vecstack_size(graphics->clipStack)) {
		ACS_SetHudClipRect(0, 0, 0, 0);
	} else {
		guiRectangle_t *top = (guiRectangle_t*)vecstack_top(graphics->clipStack);
		ACS_SetHudClipRect(top->pos.x, top->pos.y, top->width, top->height);
	}
}

void graph_setFont(guiGraphics_t *graphics, __str font)
{
	graphics->fontName = font;
	ACS_SetFont(font); 
}

void graph_drawImage(guiGraphics_t* graphics, int x, int y, __str image)
{
	#pragma fixed on
	
	int dx = x, dy = y;
	guiClipRectangle_t *top = (guiClipRectangle_t*)vecstack_top(graphics->clipStack);
	dx += top->offset.x;
	dy += top->offset.y;

	ACS_SetFont(image);
	ACS_HudMessage(HUDMSG_PLAIN, --graphics->drawOrder, CR_DARKBROWN, (fixed)dx + 0.1, (fixed)dy + 0.1, 0.03k, 0.0, 0.0, 1.0, s"A");
	ACS_SetFont(graphics->fontName);
}

/**
 * @brief                  Draws image at dstX, dstY and fits it in dstWidth, dstHeight rectangle
 * @param graphics         Graphics object
 * @param dstX             Destination x coord
 * @param dstY             Destination y coord
 * @param srcWidth         Original image width
 * @param srcHeight        Original image height
 * @param dstWidth         Scaled image width
 * @param dstHeight        Scaled image height
 * @param image            Image itself
 */
void graph_drawImageScaled(guiGraphics_t* graphics, int dstX, int dstY, 
	int srcWidth, int srcHeight, int dstWidth, int dstHeight, __str image)
{
	#pragma fixed on
	int screenWidth = graph_getScreenWidth(graphics);
	int screenHeight = graph_getScreenHeight(graphics);
	guiClipRectangle_t *top = (guiClipRectangle_t*)vecstack_top(graphics->clipStack);
	int x = dstX;
	int y = dstY;
	x += top->offset.x;
	y += top->offset.y;
	
	fixed scaleX = (fixed)srcWidth / (fixed)dstWidth;
	fixed scaleY = (fixed)srcHeight / (fixed)dstHeight;
	int w = graph_getScreenWidth(graphics) * scaleX;
	int h = graph_getScreenHeight(graphics) * scaleY;
	
	ACS_SetHudSize(w, h, 1);
	guiDebugPrint("scale (%k %k) hud %d, %d" _C_ scaleX _C_ scaleY _C_ w _C_ h);
	x *= scaleX;
	y *= scaleY;
	guiDebugPrint("drawing at (%d %d)" _C_ x _C_ y );
	
	ACS_SetHudClipRect(top->rect.pos.x * scaleX, top->rect.pos.y * scaleY, top->rect.width * scaleX, top->rect.height * scaleY);
	ACS_SetFont(image);
	ACS_HudMessage(HUDMSG_PLAIN, --graphics->drawOrder, CR_DARKBROWN, (fixed)x + 0.1, (fixed)y + 0.1, 0.03k, 0.0, 0.0, 1.0, s"A");
	ACS_SetFont(graphics->fontName);
	ACS_SetHudSize(screenWidth, screenHeight, 1);
	ACS_SetHudClipRect(top->rect.pos.x, top->rect.pos.y, top->rect.width, top->rect.width);
}

void graph_drawText(guiGraphics_t* graphics, int x, int y, const char* format, ...)
{
	va_list args;
	
	ACS_BeginStrParam();
    va_start (args, format);
    __vnprintf(format, args);
    va_end (args);
    __str text = ACS_EndStrParam();
	
	// Get actual drawing coordinates
	int dx = x, dy = y;
	guiClipRectangle_t *top = (guiClipRectangle_t*)vecstack_top(graphics->clipStack);
	dx += top->offset.x;
	dy += top->offset.y;
	guiDebugPrint("drawing text at (%d %d) %d, %d" _C_ x _C_ y _C_ dx _C_ dy);
	ACS_HudMessage(HUDMSG_PLAIN, --graphics->drawOrder, CR_BLACK, (fixed)dx + 0.1, (fixed)dy + 0.1, 0.03k, 0.0, 0.0, 1.0, text); 
}

int graph_getScreenWidth(guiGraphics_t *graphics)
{
	return graphics->screenWidth;
}

int graph_getScreenHeight(guiGraphics_t *graphics)
{
	return graphics->screenHeight;
}

