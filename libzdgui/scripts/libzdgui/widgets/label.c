#include "system.h"

#include "widgets/label.h"

#include <ctype.h>
#include "util/util.h"
#include "fonts.h"

guiLabel_vf_t guiLabel_vtable = {
	label_typename,
	label_destructor, 
	widget_getChildrenArea,
	widget_getWidgetAt,
	label_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager,
	widget_getMinimalSize
};

const char *LabelType = "Label";

const char *label_typename(guiLabel *widget)
{
	return LabelType;
}

guiLabel *label_new(guiGUI *gui, const string_t *caption, const guiFont *font)
{
	guiLabel *label = new (guiLabel);
	label_init(label, caption, font);
	gui_addWidget(gui, label);
	return label;
}

void label_destructor(guiLabel *label)
{
	// Destruction //FIXME

}

void label_init(guiLabel *label, const string_t *caption, const guiFont *font)
{
	widget_init((guiWidget *)label);
	label->widget.v = (guiWidget_vf_t *)&guiLabel_vtable;

	label->isMultiline = false;
	label->horizAlign = ALIGN_LEFT;
	label->vertAlign = ALIGN_TOP;
	label->caption = string_new_string(caption);
	label->textWrap = string_new_count(string_size(caption) * 2);
	label->wrapLines = 0;
	widget_setFont(label, (guiFont *)font);

	label_setCaption(label, caption);
	widget_addDimensionListener(&label->widget, DE_RESIZED, label_resized);

}

const string_t *label_getCaption(const guiLabel *label)
{
	return label->caption;
}

void label_setCaption(guiLabel *label, const string_t *caption)
{
	string_copy(label->caption, caption, 0, string_size(caption));
	if (label->isMultiline) {
		if (label->textWrap) {
			string_resize(label->textWrap, string_size(caption) * 2);
		}
		label_wordWrap(label);
	}
}

/**
 * @brief Adjusts the label's size to fit the caption size.
 *
 * Use this method to quickly change actual widget dimensions, so label will 
 * use less space in container.
 *
 * @param label Label which needs size adjustment
 */
void label_adjustSize(guiLabel *label)
{
	int width = 0;
	int cur = 0;
	if (label->isMultiline) {
		for (int i = 0; i < label->wrapLines; ++i) {
			int w = font_getWidthChar(widget_getFont(label), 
				(const char *)label->textWrap->s[cur], true);
			cur += w / font_getCharHeight(*widget_getFont(label)) + 1;
			if (width < w) {
				width = MIN(w, widget_getWidth(label));
			}
		}
		widget_setSize((guiWidget *)label, width, font_getCharHeight(*widget_getFont((guiWidget *)label)) * label->wrapLines);
	} else {
		widget_setSize((guiWidget *)label, font_getWidthString(widget_getFont(label), label->textWrap, false),
			font_getCharHeight(*widget_getFont((guiWidget *)label)));
	}
}


void label_draw(const guiLabel *label, guiGraphics *graphics)
{
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
			textX = widget_getWidth((guiWidget *)label) / 2;
			break;
		case ALIGN_RIGHT:
			textX = widget_getWidth((guiWidget *)label);
			break;
		default:
			textX = 0;
			guiWarning("Unknown alignment.");
	}
	if (label->isMultiline) {
		switch (label_getVerticalAlignment(label)) {
			case ALIGN_TOP:
				textY = 0;
				break;
			case ALIGN_CENTER:
				textY = (widget_getHeight(label) - label->wrapLines * font_getCharHeight(*widget_getFont(label))) / 2;
				break;
			case ALIGN_BOTTOM:
				textY = widget_getHeight(label) - label->wrapLines * font_getCharHeight(*widget_getFont(label));
				break;
			default:
				textY = 0;
				guiWarning("Unknown alignment.");
		}
	}
	
	
	if (label->isMultiline) {
		graph_drawText(graphics, widget_getFont(label), textX, textY, widget_getFontColor(label), label->textWrap->s);
	} else {
		graph_drawText(graphics, widget_getFont(label), textX, textY, widget_getFontColor(label), label->caption->s);
	} 
}

static void label_wordWrap(guiLabel *label)
{
	if (!widget_getFont(label)) {
		return;
	}
	
	guiFont *font = widget_getFont(label);
	int lineWidth = widget_getWidth(label);
	const string_t *caption = label_getCaption(label);
	
	if (!lineWidth) {
		guiError("Label has no width");
		return;
	}
	label->wrapLines = 0;
	int i = 0, j = 0, counter = 0;

	while (i < string_size(caption)) {
		for (counter = 0; counter + font_getCharWidth(*font) < lineWidth; counter += font_getCharWidth(*font)) {
			if (i == string_size(caption) - 1) {
				label->textWrap->s[i] = '\0';
				return;
			}
			label->textWrap->s[i] = caption->s[i];
			if (label->textWrap->s[i] == '\n') {
				counter = 0;
			}
			++i;
		}

		if (isspace(caption->s[i])) {
			label->textWrap->s[i] = '\n';
			++i;
			++label->wrapLines;
		} else {
			for (j = i; j >= 0; --j) {
				if (isspace(caption->s[j])) {
					label->textWrap->s[j] = '\n';
					i = j + 1;
					++label->wrapLines;
					break;
				}
			}
		}
	}
	label->textWrap->s[i] = '\0';
}

void label_resized(void *widget, guiDimensionEvent *dimEvent)
{
	guiLabel *label = (guiLabel*)widget;
	if (label->isMultiline) {
		label_wordWrap(label);
	}
}

