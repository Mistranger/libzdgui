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

struct guiImageWidget;

extern const char *ImageWidgetType;

typedef struct guiImageWidget_vf {
	const char *(*w_typename)(struct guiImageWidget *widget);
	void (*w_destructor)(struct guiWidget *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void (*w_draw)(const struct guiImageWidget *image, guiGraphics *graphics);
	void (*w_tick)(struct guiWidget *widget);
	bool (*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiWidget *widget, void *focus);
	struct vec2i* (*w_getMinimalSize)(const struct guiWidget *widget);
} guiImageWidget_vf_t;

typedef struct guiImageWidget {
	guiWidget widget;
	guiImage image;
	enum ImageFlags imageFlags;
} guiImageWidget;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
 * @brief              Constructor (image initialization with caption)
 * @param caption      title caption
 */
guiImageWidget *wimage_new(guiGUI *gui, __str filename, int imageWidth, int imageHeight);
void wimage_init(guiImageWidget *image, __str filename, int imageWidth, int imageHeight);

// Virtual inherited from guiWidget
const char *wimage_typename(guiImageWidget *widget);
void wimage_draw(const guiImageWidget *widget, guiGraphics *graphics);

#endif // WIDGETS_IMAGE_H_INCLUDED
