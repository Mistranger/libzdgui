#include "system.h"
#include "fonts.h"

const guiFont_t FONT_CONFONT = {8, 8, s"CONFONT"};

guiFont_t* font_new(__str fontImage, int charWidth, int charHeight)
{
	guiFont_t *font = new(guiFont_t);
	font->f = fontImage;
	font->charWidth = charWidth;
	font->charHeight = charHeight;
	return font;
}

int font_getWidthChar(const guiFont_t* font, const char* text)
{
	const size_t l = strlen(text);
	size_t size = 0;
	for (size_t i = 0; i < l; ++i) {
		size += font->charWidth;
	}
	return size;
}

inline int font_getWidthString(const guiFont_t* font, const string_t* text)
{
	return font_getWidthChar(font, string_cstr(text));
}

inline int font_getCharWidth(const guiFont_t* font)
{
	return font->charWidth;
}

inline int font_getCharHeight(const guiFont_t* font)
{
	return font->charHeight;
}
