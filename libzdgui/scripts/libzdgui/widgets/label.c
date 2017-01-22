#include "system.h"

#include "widgets/label.h"

#include "util/util.h"

guiLabel_vf_t guiLabel_vtable = {
	widget_getChildrenArea,
	widget_getWidgetAt,
	label_draw,
	widget_tick,
	widget_isWidgetExisting
};

guiLabel_t* label_new(const string_t *caption)
{
	guiLabel_t *label = (guiLabel_t*)malloc(sizeof(guiLabel_t));
	label_init(label, caption);
	return label;
}

void label_init(guiLabel_t *label, const string_t *caption)
{
	widget_init((guiWidget_t*)label);
	label->widget.v = (guiWidget_vf_t*)&guiLabel_vtable;
	
	label->caption = string_new_string(caption);
	
	label_setCaption(label, caption);
}

const string_t* label_getCaption(const guiLabel_t* label)
{
	return label->caption;
}

void label_setCaption(guiLabel_t* label, const string_t *caption)
{
	if (label->caption) {
		string_delete(label->caption);
	}
	label->caption = string_new_string(caption);
}

labelAlignment_t label_getAlignment(const guiLabel_t* label)
{
	return label->alignment;
}

void label_setAlignment(guiLabel_t* label, labelAlignment_t align)
{
	label->alignment = align;
}

void label_adjustSize(guiLabel_t* label)
{
	int width = 0;
	for (int i = 0; i < vector_size(label->textWrap); ++i) {
		int w = font_getWidthString(widget_getFont((guiWidget_t*)label), vector_get(label->textWrap, i, string_t*));
		if (width < w) {
			width = MIN(w, widget_getWidth((guiWidget_t*)label));
		}
	}
	widget_setSize((guiWidget_t*)label, width, font_getCharHeight(widget_getFont((guiWidget_t*)label)) * vector_size(label->textWrap));
}


void label_draw(const guiLabel_t* label, guiGraphics_t* graphics)
{
	int textX, textY;
	switch (label_getAlignment(label)) {
		case ALIGN_LEFT:
			textX = 0;
			break;
		case ALIGN_CENTER:
			textX = widget_getWidth((guiWidget_t*)label) / 2;
			break;
		case ALIGN_RIGHT:
			textX = widget_getWidth((guiWidget_t*)label);
			break;
	}
	
}

static void label_wordWrap(guiLabel_t* label)
{
	guiFont_t *font = widget_getFont((guiWidget_t*)label);
	int lineWidth = widget_getWidth((guiWidget_t*)label);
	string_t *str = string_new_string(label_getCaption(label));
	size_t pos, lastPos;
	string_t *substr;
	bool done = false;
	bool first = true;

	vector_clear(label->textWrap);

	while (!done) {
		if (string_find_c(str, '\n', 0) != STRING_NPOS || font_getWidthString(font, str) > lineWidth) {
			// string too wide or has a newline, split it up
			first = true;
			lastPos = 0;
			while (1) {
				// look for any whitespace
				pos = string_find_first_of_char(str, " \t\n", first ? 0 : lastPos + 1);
				if (pos != STRING_NPOS) {
					// found space, now check width
					substr = string_substr(str, 0, pos);
					if (font_getWidthString(font, substr) > lineWidth) {
						// sub-string is too big, use last good position
						if (first) {
							// didn't find a good last position
							substr = string_substr(str, 0, pos);
							vector_push_back(label->textWrap, substr);
							str = string_substr(str, pos + 1, STRING_NPOS);
							break;
						} else {
							substr = string_substr(str, 0, lastPos);
							vector_push_back(label->textWrap, substr);
							// If we stopped at a space then skip any extra spaces but stop at a newline
							if (string_get(str, lastPos) != '\n') {
								while (string_get(str, lastPos + 1) == ' ' 
									|| string_get(str, lastPos + 1) == '\t' || string_get(str, lastPos + 1) == '\n') {
										++lastPos;
										if (string_get(str, lastPos) == '\n') {
											break;
										}
								}
							}
							str = string_substr(str, lastPos + 1, STRING_NPOS);
							break;
						}
					} else {
						// sub-string is small enough
						// stop if we found a newline, otherwise look for next space
						if (string_get(str, pos) == '\n') {
							substr = string_substr(str, 0, pos);
							vector_push_back(label->textWrap, substr);
							str = string_substr(str, pos + 1, STRING_NPOS);
							break;
						}
					}
				} else {
					// no space found
					if (first) {
						// didn't find a good last position, we're done
						vector_push_back(label->textWrap, str);
						done = true;
						break;
					} else {
						substr = string_substr(str, 0, lastPos);
						vector_push_back(label->textWrap, substr);
						str = string_substr(str, lastPos + 1, STRING_NPOS);
						break;
					}
				}
				lastPos = pos;
				first = false;
			}
		} else {
			// string small enough
			vector_push_back(label->textWrap, str);
			done = true;
		}
	}
}

