#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED


/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

// Maximum length of label's text
#define MAX_CAPTION_LENGTH 256

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

enum LabelAlignment {
	LEFT = 0,
	CENTER,
	RIGHT
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
	char caption[MAX_CAPTION_LENGTH];
	size_t padding;
	enum LabelAlignment alignment;
} guiLabel_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
 * @brief              Constructor (label initialization with caption)
 * @param caption      caption text
 */
guiLabel_t* label_new(const char *caption);
void label_init(guiLabel_t *label, const char *caption);


// Virtual inherited from guiWidget_t

void label_draw(const guiLabel_t *widget, guiGraphics_t *graphics);

/**
 * @brief				Sets label's caption
 * @param label
 * @param caption
 */
void label_setCaption(guiLabel_t *label, const char *caption);

/**
 * @brief 				Gets label's caption
 * @param label
 */
const char* label_getCaption(const guiLabel_t *label);


#endif // DEFS_H_INCLUDED
