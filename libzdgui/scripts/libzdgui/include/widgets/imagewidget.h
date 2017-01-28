#ifndef WIDGETS_IMAGE_H_INCLUDED
#define WIDGETS_IMAGE_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "gui.h"
#include "image.h"
#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

enum ImageFlags {
	IF_SCALETOSIZE = 0x00000001,
};

struct guiImageWidget_s;

extern const char *ImageWidgetType;

typedef struct guiImageWidget_vf {
	const char *(*w_typename)(struct guiImageWidget_s *widget);
	void (*w_destructor)(struct guiWidget_s *widget);
	struct guiRectangle_s *(*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s *(*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void (*w_draw)(const struct guiImageWidget_s *image, guiGraphics_t *graphics);
	void (*w_tick)(struct guiWidget_s *widget);
	bool (*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
	void(*w_setFocusHandler)(struct guiWidget_s *widget, void *focus);
} guiImageWidget_vf_t;

typedef struct guiImageWidget_s {
	guiWidget_t widget;
	guiImage_t image;
	enum ImageFlags imageFlags;
} guiImageWidget_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
 * @brief              Constructor (image initialization with caption)
 * @param caption      title caption
 */
guiImageWidget_t *wimage_new(guiGUI_t *gui, __str filename, int imageWidth, int imageHeight);
void wimage_init(guiImageWidget_t *image, __str filename, int imageWidth, int imageHeight);

// Virtual inherited from guiWidget_t
const char *wimage_typename(guiImageWidget_t *widget);
void wimage_draw(const guiImageWidget_t *widget, guiGraphics_t *graphics);

#endif // WIDGETS_IMAGE_H_INCLUDED
