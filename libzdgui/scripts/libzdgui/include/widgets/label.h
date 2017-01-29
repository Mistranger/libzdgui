#ifndef WIDGETS_LABEL_H_INCLUDED
#define WIDGETS_LABEL_H_INCLUDED


/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "gui.h"
#include "graphics.h"
#include "widget.h"
#include "event/event_dimension.h"
#include "util/string.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

extern const char *LabelType;

struct guiLabel;

typedef struct guiLabel_vf {
	const char *(*w_typename)(struct guiLabel *widget);
	void (*w_destructor)(struct guiLabel *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void (*w_draw)(const struct guiLabel *container, guiGraphics *graphics);
	void (*w_tick)(struct guiWidget *widget);
	bool (*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiWidget *widget, void *focus);
	struct vec2i* (*w_getMinimalSize)(const struct guiWidget *widget);
} guiLabel_vf_t;

typedef struct guiLabel {
	guiWidget widget;
	string_t *caption;
	string_t *textWrap;
	int wrapLines;
	bool isMultiline;
	guiAlignment horizAlign;
	guiAlignment vertAlign;
} guiLabel;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/


/**
 * @brief              Constructor (label initialization with caption)
 * @param caption      caption text
 */
guiLabel *label_new(guiGUI *gui, const string_t *caption, const guiFont *font);
void label_destructor(guiLabel *label);
void label_init(guiLabel *label, const string_t *caption, const guiFont *font);


// Virtual inherited from guiWidget
const char *label_typename(guiLabel *widget);
void label_draw(const guiLabel *widget, guiGraphics *graphics);


void label_setCaption(guiLabel *label, const string_t *caption);
const string_t *label_getCaption(const guiLabel *label);

#define label_getAlignment(_label) (((guiLabel*)_label)->horizAlign)
#define label_setAlignment(_label, _align) { ((guiLabel*)_label)->horizAlign = _align; }

#define label_getVerticalAlignment(_label) (((guiLabel*)_label)->vertAlign)
#define label_setVerticalAlignment(_label, _align) { ((guiLabel*)_label)->vertAlign = _align; }

void label_adjustSize(guiLabel *label);
static void label_wordWrap(guiLabel *label);

// Event listeners

void label_resized(void *widget, guiDimensionEvent *dimEvent);


#endif // WIDGETS_LABEL_H_INCLUDED
