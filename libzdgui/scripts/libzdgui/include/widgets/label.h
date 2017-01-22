#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED


/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "widget.h"
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
	ALIGN_RIGHT
} labelAlignment_t;

// Label flags
enum LabelFlags
{
	LF_MULTILINE = 0x00000001,
};

struct guiLabel_s;

typedef struct guiLabel_vf {
	
	struct guiRectangle_s (*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, int x, int y);
	void (*w_draw)(const struct guiLabel_s *container, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
} guiLabel_vf_t;

typedef struct guiLabel_s
{
    guiWidget_t widget;
	string_t *caption;
	vector_t *textWrap;
	size_t padding;
	labelAlignment_t alignment;
} guiLabel_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
 * @brief              Constructor (label initialization with caption)
 * @param caption      caption text
 */
guiLabel_t* label_new(const string_t *caption);
void label_init(guiLabel_t *label, const string_t *caption);


// Virtual inherited from guiWidget_t

void label_draw(const guiLabel_t *widget, guiGraphics_t *graphics);


void label_setCaption(guiLabel_t *label, const string_t *caption);
const string_t* label_getCaption(const guiLabel_t* label);

void label_setAlignment(guiLabel_t *label, labelAlignment_t align);
labelAlignment_t label_getAlignment(const guiLabel_t* label);

void label_adjustSize(guiLabel_t *label);
static void label_wordWrap(guiLabel_t *label);


#endif // DEFS_H_INCLUDED
