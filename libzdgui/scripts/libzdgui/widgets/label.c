#include "system.h"

#include "widgets/label.h"

#include <ctype.h>
#include "util/util.h"
#include "fonts.h"

guiLabel_vf_t guiLabel_vtable = {
	label_typename,
	widget_destructor, //FIXME
	widget_getChildrenArea,
	widget_getWidgetAt,
	label_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager
};

const char *LabelType = "Label";

const char *label_typename(guiLabel_t *widget)
{
	return LabelType;
}

guiLabel_t *label_new(guiGUI_t *gui, const string_t *caption, const guiFont_t *font)
{
	guiLabel_t *label = new (guiLabel_t);
	label_init(label, caption, font);
	gui_addWidget(gui, label);
	return label;
}

void label_init(guiLabel_t *label, const string_t *caption, const guiFont_t *font)
{
	widget_init((guiWidget_t *)label);
	label->widget.v = (guiWidget_vf_t *)&guiLabel_vtable;

	label->labelFlags = 0;
	label->horizAlign = ALIGN_LEFT;
	label->vertAlign = ALIGN_TOP;
	label->caption = string_new_string(caption);
	label->textWrap = NULL;
	widget_setFont(label, (guiFont_t *)font);

	label_setCaption(label, caption);
	widget_addDimensionListener(&label->widget, DE_RESIZED, label_resized);

}

const string_t *label_getCaption(const guiLabel_t *label)
{
	return label->caption;
}

void label_setCaption(guiLabel_t *label, const string_t *caption)
{
	string_copy(label->caption, caption, 0, string_size(caption));
	if (label->labelFlags & LF_MULTILINE) {
		if (label->textWrap) {
			string_resize(label->textWrap, string_size(label->caption) * 2);
		} else {
			label->textWrap = string_new_count(string_size(label->caption));
		}
		label_wordWrap(label);
	}
}

void label_adjustSize(guiLabel_t *label)
{/*
	int width = 0;
	for (int i = 0; i < label->textWrapCount; ++i) {
		int w = font_getWidthString(widget_getFont((guiWidget_t *)label), (const string_t *)vector_at(label->textWrap, i));
		if (width < w) {
			width = MIN(w, widget_getWidth((guiWidget_t *)label));
		}
	}
	widget_setSize((guiWidget_t *)label, widget_getWidth((guiWidget_t *)label), font_getCharHeight(*widget_getFont((guiWidget_t *)label)));*/
}


void label_draw(const guiLabel_t *label, guiGraphics_t *graphics)
{/*
	if (!widget_getFont(label)) {
		guiError("Label doesn't have font set!");
		return;
	}

	int textX, textY;
	switch (label_getAlignment(label)) {
		case ALIGN_LEFT:
			textX = 0;
			break;
		case ALIGN_CENTER:
			textX = widget_getWidth((guiWidget_t *)label) / 2;
			break;
		case ALIGN_RIGHT:
			textX = widget_getWidth((guiWidget_t *)label);
			break;
		default:
			textX = 0;
			guiWarning("Unknown alignment.");
	}
	switch (label_getVerticalAlignment(label)) {
		case ALIGN_TOP:
			textY = 0;
			break;
		case ALIGN_CENTER:
			textY = (widget_getHeight(label) - vector_size(label->textWrap) * font_getCharHeight(*widget_getFont(label))) / 2;
			break;
		case ALIGN_BOTTOM:
			textY = widget_getHeight(label) - vector_size(label->textWrap) * font_getCharHeight(*widget_getFont(label));
			break;
		default:
			textY = 0;
			guiWarning("Unknown alignment.");
			//throw GCN_EXCEPTION("Unknown alignment.");
	}
	guiDebugPrint("drawing label.");

	graph_drawText(graphics, widget_getFont(label), textX, textY, label->textWrap->s);*/
}

static void label_wordWrap(guiLabel_t *label)
{
	if (!widget_getFont(label)) {
		return;
	}

	guiFont_t *font = widget_getFont(label);
	int lineWidth = widget_getWidth(label);
	const string_t *caption = label_getCaption(label);
	/*
	string_t *str = string_new_string(label_getCaption(label));
	size_t pos, lastPos;
	string_t *substr = string_new();
	string_t *subcopy = string_new();
	bool done = false;
	bool first = true;
	*/
	size_t i = 0, j, counter;

	while (i < string_size(caption)) {
		// copy string until the end of the line is reached
		for (counter = 0; counter < lineWidth; counter++) {
			// check if end of string reached
			if (i == string_size(caption) - 1) {
				label->textWrap->s[i] = '\0';
				return;
			}
			label->textWrap->s[i] = caption->s[i];
			// check for newlines embedded in the original input
			// and reset the index
			if (label->textWrap->s[i] == '\n') {
				counter = 0;
			}
			++i;
		}
		// check for whitespace
		if (isspace(caption->s[i])) {
			label->textWrap->s[i] = '\n';
			++i;
		} else {
			// check for nearest whitespace back in string
			for (j = i; j >= 0; --j) {
				if (isspace(caption->s[i])) {
					label->textWrap->s[j] = '\n';
					// set string index back to character after this one
					i = j + 1;
					break;
				}
			}
		}
	}
	label->textWrap->s[i] = '\0';
	/*

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
					string_assign_string(substr, string_substr(str, 0, pos));

					if (font_getWidthString(font, substr) > lineWidth) {
						// sub-string is too big, use last good position

						if (first) {
							// didn't find a good last position
							string_assign_string(substr, string_substr(str, 0, pos));
							subcopy = string_new_string(substr);
							vector_push_back(label->textWrap, subcopy);
							string_assign_string(str, string_substr(str, pos + 1, STRING_NPOS));
							break;
						} else {
							string_assign_string(substr, string_substr(str, 0, lastPos));
							subcopy = string_new_string(substr);
							vector_push_back(label->textWrap, subcopy);
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
							string_assign_string(str, string_substr(str, lastPos + 1, STRING_NPOS));
							break;
						}
					} else {
						// sub-string is small enough
						// stop if we found a newline, otherwise look for next space
						if (string_get(str, pos) == '\n') {
							string_assign_string(substr, string_substr(str, 0, pos));
							subcopy = string_new_string(substr);
							vector_push_back(label->textWrap, subcopy);
							string_assign_string(str, string_substr(str, pos + 1, STRING_NPOS));
							break;
						}

					}
				} else {
					// no space found
					if (first) {
						// didn't find a good last position, we're done
						subcopy = string_new_string(str);
						vector_push_back(label->textWrap, subcopy);
						done = true;
						break;
					} else {
						string_assign_string(substr, string_substr(str, 0, lastPos));
						subcopy = string_new_string(substr);
						vector_push_back(label->textWrap, subcopy);
						string_assign_string(str, string_substr(str, lastPos + 1, STRING_NPOS));
						break;
					}
				}
				lastPos = pos;
				first = false;
			}
		} else {
			// string small enough
			subcopy = string_new_string(str);
			vector_push_back(label->textWrap, subcopy);
			done = true;
		}
	}*/
}

void label_resized(void *widget, dimensionEvent_t *dimEvent)
{
	label_wordWrap((guiLabel_t *)widget);
}

