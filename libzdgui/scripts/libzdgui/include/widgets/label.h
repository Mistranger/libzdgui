#ifndef WIDGETS_LABEL_H_INCLUDED
#define WIDGETS_LABEL_H_INCLUDED


/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "gui.h"
#include "widget.h"
#include "event/event_dimension.h"
#include "util/string.h"
#include "util/vector.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef enum labelAlignment {
	ALIGN_LEFT = 0,
	ALIGN_CENTER,
	ALIGN_RIGHT,
	ALIGN_TOP,
	ALIGN_BOTTOM,
} labelAlignment_t;

// Label flags
typedef enum LabelFlags {
	LF_MULTILINE = 0x00000001,
} LabelFlags_t;

extern const char *LabelType;

struct guiLabel_s;

typedef struct guiLabel_vf {
	const char *(*w_typename)(struct guiLabel_s *widget);
	void (*w_destructor)(struct guiWidget_s *widget);
	struct guiRectangle_s *(*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s *(*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void (*w_draw)(const struct guiLabel_s *container, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
	void(*w_setFocusHandler)(struct guiWidget_s *widget, void *focus);
} guiLabel_vf_t;

typedef struct guiLabel_s {
	guiWidget_t widget;
	string_t *caption;
	string_t *textWrap;
	int textWrapCount;
	LabelFlags_t labelFlags;
	labelAlignment_t horizAlign;
	labelAlignment_t vertAlign;
} guiLabel_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/


/**
 * @brief              Constructor (label initialization with caption)
 * @param caption      caption text
 */
guiLabel_t *label_new(guiGUI_t *gui, const string_t *caption, const guiFont_t *font);
void label_init(guiLabel_t *label, const string_t *caption, const guiFont_t *font);


// Virtual inherited from guiWidget_t
const char *label_typename(guiLabel_t *widget);
void label_draw(const guiLabel_t *widget, guiGraphics_t *graphics);


void label_setCaption(guiLabel_t *label, const string_t *caption);
const string_t *label_getCaption(const guiLabel_t *label);

#define label_getAlignment(_label) (((guiLabel_t*)_label)->horizAlign)
#define label_setAlignment(_label, _align) { ((guiLabel_t*)_label)->horizAlign = _align; }

#define label_getVerticalAlignment(_label) (((guiLabel_t*)_label)->vertAlign)
#define label_setVerticalAlignment(_label, _align) { ((guiLabel_t*)_label)->vertAlign = _align; }

void label_adjustSize(guiLabel_t *label);
static void label_wordWrap(guiLabel_t *label);

// Event listeners

void label_resized(void *widget, dimensionEvent_t *dimEvent);


#endif // WIDGETS_LABEL_H_INCLUDED
