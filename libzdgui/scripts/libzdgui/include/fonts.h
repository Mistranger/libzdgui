#ifndef FONTS_H_INCLUDED
#define FONTS_H_INCLUDED

#include "util/string.h"

typedef struct guiFont {
	int charWidth, charHeight;
	
	__str f;
} guiFont;

extern const guiFont FONT_CONFONT;
extern const guiFont FONT_CFONT8;
extern const guiFont FONT_CFONT10;

guiFont *font_new(__str font, int charWidth, int charHeight);
int font_getWidthChar(const guiFont *font, const char *text, bool checkNewline);
#define font_getWidthString(_font, _text, _checkSpace) font_getWidthChar(_font, string_cstr(_text), _checkSpace)
#define font_getCharWidth(_font) ((_font).charWidth)
#define font_getCharHeight(_font) ((_font).charHeight)

#endif // FONTS_H_INCLUDED
