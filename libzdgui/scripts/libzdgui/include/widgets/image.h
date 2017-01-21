#ifndef WIDGETS_IMAGE_H_INCLUDED
#define WIDGETS_IMAGE_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

enum ImageFlags
{
	IF_SCALETOSIZE = 0x00000001,
};

struct guiImage_s;

typedef struct guiImage_vf {
	struct guiRectangle_s (*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, int x, int y);
	void (*w_draw)(const struct guiImage_s *image, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
} guiImage_vf_t;

typedef struct guiImage_s
{
    guiWidget_t widget;
	int imageWidth, imageHeight;
	enum ImageFlags imageFlags;
	str filename;
} guiImage_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
 * @brief              Constructor (image initialization with caption)
 * @param caption      title caption
 */
guiImage_t* image_new(str filename, int imageWidth, int imageHeight);
void image_init(guiImage_t *image, str filename, int imageWidth, int imageHeight);

// Virtual inherited from guiWidget_t
void image_draw(const guiImage_t *widget, guiGraphics_t *graphics);

#endif // WIDGETS_IMAGE_H_INCLUDED
