#include "system.h"
#include "fonts.h"

const guiFont FONT_CONFONT = {8, 8, s"CONFONT"};
const guiFont FONT_CFONT8 = {7, 14, s"CFONT8"};
const guiFont FONT_CFONT10 = {8, 16, s"CFONT10"};

guiFont *font_new(__str fontImage, int charWidth, int charHeight)
{
	guiFont *font = new (guiFont);
	font->f = fontImage;
	font->charWidth = charWidth;
	font->charHeight = charHeight;
	return font;
}

int font_getWidthChar(const guiFont *font, const char *text, bool checkNewline)
{
	size_t size = 0;
	for (size_t i = 0; text[i]; ++i) {
		if (checkNewline && text[i] == '\n') {
			return size;
		}
		size += font->charWidth;
	}
	return size;
}
