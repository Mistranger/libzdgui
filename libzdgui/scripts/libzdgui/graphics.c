#include "system.h"
#include "graphics.h"
#include "image.h"

void graph_init(guiGraphics *graphics, int width, int height)
{
	graphics->screenWidth = width;
	graphics->screenHeight = height;
	graphics->clipStack = vecstack_init(sizeof(guiClipRectangle));
	graphics->drawOrder = HUDMESSAGE_ID;
	ACS_SetHudSize(width, height, 1);
}

void graph_beginDraw(guiGraphics *graphics)
{
	graphics->drawOrder = HUDMESSAGE_ID;
	guiRectangle r = {0, 0, graph_getScreenWidth(graphics), graph_getScreenHeight(graphics)};
	graph_pushClipArea(graphics, r);
}

void graph_endDraw(guiGraphics *graphics)
{
	graph_popClipArea(graphics);
}

void graph_pushClipArea(guiGraphics *graphics, const guiRectangle area)
{
	if (!vecstack_size(graphics->clipStack)) {
		guiClipRectangle clip;
		clip.rect.pos = area.pos;
		clip.rect.width = area.width;
		clip.rect.height = area.height;
		clip.offset.x = 0;
		clip.offset.y = 0;
		ACS_SetHudClipRect(clip.rect.pos.x, clip.rect.pos.y, clip.rect.width, clip.rect.height);
		vecstack_push(graphics->clipStack, &clip);
		guiDebugPrint("pushing (%d,%d,%d,%d) rect to clip stack, %d in stack" _C_ clip.rect.pos.x _C_ clip.rect.pos.y _C_ clip.rect.width _C_ clip.rect.height _C_ vecstack_size(graphics->clipStack));
		return;
	}

	guiClipRectangle clip;
	guiClipRectangle *top = ((guiClipRectangle *)(vecstack_top(graphics->clipStack)));
	memcpy(&clip, &area, sizeof(guiRectangle));
	clip.offset.x = top->offset.x + clip.rect.pos.x;
	clip.offset.y = top->offset.y + clip.rect.pos.y;
	clip.rect.pos.x += top->offset.x;
	clip.rect.pos.y += top->offset.y;

	if (clip.rect.pos.x < top->rect.pos.x) {
		clip.rect.pos.x = top->rect.pos.x;
	}

	if (clip.rect.pos.y < top->rect.pos.y) {
		clip.rect.pos.y = top->rect.pos.y;
	}


	if (clip.rect.width > top->rect.width) {
		clip.rect.width = top->rect.width;
	}

	if (clip.rect.height > top->rect.height) {
		clip.rect.height = top->rect.height;
	}

	rect_intersect((guiRectangle *)&clip, (guiRectangle *)top);
	vecstack_push(graphics->clipStack, &clip);
	guiDebugPrint("pushing (%d,%d,%d,%d) rect to clip stack, %d in stack" _C_ clip.rect.pos.x _C_ clip.rect.pos.y _C_ clip.rect.width _C_ clip.rect.height _C_ vecstack_size(graphics->clipStack));
	ACS_SetHudClipRect(clip.rect.pos.x, clip.rect.pos.y, clip.rect.width, clip.rect.height);

}

void graph_popClipArea(guiGraphics *graphics)
{
	if (!graphics->clipStack) {
		guiError("No clipStack");
		return;
	}

	guiClipRectangle *clip = (guiClipRectangle *)vecstack_top(graphics->clipStack);
	guiDebugPrint("pop (%d,%d,%d,%d) rect from clip stack, %d in stack" _C_ clip->rect.pos.x _C_ clip->rect.pos.y _C_ clip->rect.width _C_ clip->rect.height _C_ vecstack_size(graphics->clipStack));
	vecstack_pop(graphics->clipStack);
	if (!vecstack_size(graphics->clipStack)) {
		ACS_SetHudClipRect(0, 0, 0, 0);
	} else {
		guiRectangle *top = (guiRectangle *)vecstack_top(graphics->clipStack);
		ACS_SetHudClipRect(top->pos.x, top->pos.y, top->width, top->height);
	}
}

void graph_setFont(guiGraphics *graphics, __str font)
{
	graphics->fontName = font;
	ACS_SetFont(font);
}

void graph_drawRect(guiGraphics *graphics, int x, int y, int width, int height, __str color)
{
	guiClipRectangle *top = (guiClipRectangle *)vecstack_top(graphics->clipStack);
	int dx = x + top->offset.x;
	int dy = y + top->offset.y;

	ACS_SetHudClipRect(dx, dy, width, height);
	ACS_SetFont(s"HUDFONT_libzdgui_BACK");
	ACS_HudMessageColor(HUDMSG_PLAIN, --graphics->drawOrder, color, (fixed)dx + 0.1, (fixed)dy + 0.1, 0.03k, s"A");
	ACS_SetFont(graphics->fontName);
	ACS_SetHudClipRect(top->rect.pos.x, top->rect.pos.y, top->rect.width, top->rect.height);
}

void graph_drawRectAlpha(guiGraphics *graphics, int x, int y, int width, int height, __str color, fixed alpha)
{
	guiClipRectangle *top = (guiClipRectangle *)vecstack_top(graphics->clipStack);
	int dx = x + top->offset.x;
	int dy = y + top->offset.y;

	ACS_SetHudClipRect(dx, dy, width, height);
	ACS_SetFont(s"HUDFONT_libzdgui_BACK");
	ACS_HudMessageColorExt(HUDMSG_PLAIN, --graphics->drawOrder, color, (fixed)dx + 0.1, (fixed)dy + 0.1, 0.03k,alpha, s"A");
	ACS_SetFont(graphics->fontName);
	ACS_SetHudClipRect(top->rect.pos.x, top->rect.pos.y, top->rect.width, top->rect.height);
}

void graph_drawImage(guiGraphics *graphics, int x, int y, __str image)
{
	int dx = x, dy = y;
	guiClipRectangle *top = (guiClipRectangle *)vecstack_top(graphics->clipStack);
	dx += top->offset.x;
	dy += top->offset.y;

	ACS_SetFont(image);
	ACS_HudMessage(HUDMSG_PLAIN, --graphics->drawOrder, CR_DARKBROWN, (fixed)dx + 0.1, (fixed)dy + 0.1, 0.03k, s"A");
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
void graph_drawImageScaled(guiGraphics *graphics, int dstX, int dstY,
						   int srcWidth, int srcHeight, int dstWidth, int dstHeight, __str image)
{
	guiClipRectangle *top = (guiClipRectangle *)vecstack_top(graphics->clipStack);
	int x = dstX + top->offset.x;
	int y = dstY + top->offset.y;

	fixed scaleX = (fixed)srcWidth / (fixed)dstWidth;
	fixed scaleY = (fixed)srcHeight / (fixed)dstHeight;

	ACS_SetHudSize(graph_getScreenWidth(graphics) * scaleX, graph_getScreenHeight(graphics) * scaleY, 1);
	x *= scaleX;
	y *= scaleY;
	guiDebugPrint("drawing at (%d %d)" _C_ x _C_ y);

	ACS_SetHudClipRect(top->rect.pos.x * scaleX, top->rect.pos.y * scaleY, top->rect.width * scaleX, top->rect.height * scaleY);
	ACS_SetFont(image);
	ACS_HudMessage(HUDMSG_PLAIN, --graphics->drawOrder, CR_DARKBROWN, (fixed)x + 0.1, (fixed)y + 0.1, 0.03k, s"A");
	ACS_SetFont(graphics->fontName);
	ACS_SetHudSize(graph_getScreenWidth(graphics), graph_getScreenHeight(graphics), 1);
	ACS_SetHudClipRect(top->rect.pos.x, top->rect.pos.y, top->rect.width, top->rect.height);
}

void graph_drawText(guiGraphics *graphics, guiFont *font, int x, int y, const char *format, ...)
{
	va_list args;

	ACS_BeginStrParam();
	va_start(args, format);
	__vnprintf(format, args);
	va_end(args);
	__str text = ACS_EndStrParam();

	guiClipRectangle *top = (guiClipRectangle *)vecstack_top(graphics->clipStack);
	ACS_SetFont(font->f);
	ACS_HudMessage(HUDMSG_PLAIN, --graphics->drawOrder, CR_BLACK, (fixed)(x + top->offset.x) + 0.1,
				   (fixed)(y + top->offset.y) + 0.1, 0.03k, text);
}

int graph_getScreenWidth(guiGraphics *graphics)
{
	return graphics->screenWidth;
}

int graph_getScreenHeight(guiGraphics *graphics)
{
	return graphics->screenHeight;
}

