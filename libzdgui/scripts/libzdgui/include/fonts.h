#ifndef FONTS_H_INCLUDED
#define FONTS_H_INCLUDED

#include "util/string.h"

typedef struct guiFont_s {
	int charWidth, charHeight;
	__str f;
} guiFont_t;

extern const guiFont_t FONT_CONFONT;

guiFont_t* font_new(__str font, int charWidth, int charHeight);
int font_getWidthChar(const guiFont_t *font, const char *text);
inline int font_getWidthString(const guiFont_t *font, const string_t *text);
inline int font_getCharWidth(const guiFont_t *font);
inline int font_getCharHeight(const guiFont_t *font);

#endif // FONTS_H_INCLUDED
