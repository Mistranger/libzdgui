#include "system.h"

#include "widgets/image.h"

guiImage_vf_t guiImage_vtable = {
	widget_getChildrenArea,
	widget_getWidgetAt,
	image_draw,
	widget_tick,
	widget_isWidgetExisting
};

guiImage_t* image_new(__str filename, int imageWidth, int imageHeight)
{
	guiImage_t *image = (guiImage_t*)malloc(sizeof(guiImage_t));
	image_init(image, filename, imageWidth, imageHeight);
	guiInfo("new image created");
	return image;
}

void image_init(guiImage_t* image, __str filename, int imageWidth, int imageHeight)
{
	widget_init(&image->widget);
	image->widget.v = (guiWidget_vf_t*)&guiImage_vtable;
	
	image->filename = filename;
	image->imageWidth = imageWidth;
	image->imageHeight = imageHeight;
	image->imageFlags = 0;
	widget_setSize((guiWidget_t *)image, imageWidth, imageHeight);
}

void image_draw(const guiImage_t* image, guiGraphics_t* graphics)
{
	guiDebugPrint("drawing image");
	if (image->imageFlags & IF_SCALETOSIZE) {
		graph_drawImageScaled(graphics, 0, 0, image->imageWidth, image->imageHeight, 
			widget_getWidth((guiWidget_t*)image), widget_getHeight((guiWidget_t*)image), image->filename);
	} else {
		graph_drawImage(graphics, 0, 0, image->filename);
	}
}

