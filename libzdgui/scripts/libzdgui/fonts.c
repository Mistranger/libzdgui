#include "system.h"
#include "fonts.h"

const guiFont FONT_CONFONT = {8, 8, s"CONFONT"};

guiFont *font_new(__str fontImage, int charWidth, int charHeight)
{
	guiFont *font = new (guiFont);
	font->f = fontImage;
	font->charWidth = charWidth;
	font->charHeight = charHeight;
	return font;
}

int font_getWidthChar(const guiFont *font, const char *text, bool checkSpace)
{
	const size_t l = strlen(text);
	size_t size = 0;
	for (size_t i = 0; i < l; ++i) {
		if (checkSpace && text[i] == '\n') {
			return size;
		}
		size += font->charWidth;
	}
	return size;
}
