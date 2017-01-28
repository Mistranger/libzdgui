#ifndef FONTS_H_INCLUDED
#define FONTS_H_INCLUDED

#include "util/string.h"

typedef struct guiFont_s {
	int charWidth, charHeight;
	__str f;
} guiFont_t;

extern const guiFont_t FONT_CONFONT;

guiFont_t *font_new(__str font, int charWidth, int charHeight);
int font_getWidthChar(const guiFont_t *font, const char *text, bool checkSpace);
#define font_getWidthString(_font, _text, _checkSpace) font_getWidthChar(_font, string_cstr(_text), _checkSpace)
#define font_getCharWidth(_font) ((_font).charWidth)
#define font_getCharHeight(_font) ((_font).charHeight)

#endif // FONTS_H_INCLUDED
