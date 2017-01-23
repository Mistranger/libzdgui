#include "system.h"

#include "widgets/imagewidget.h"

guiImageWidget_vf_t guiImage_vtable = {
	widget_getChildrenArea,
	widget_getWidgetAt,
	wimage_draw,
	widget_tick,
	widget_isWidgetExisting
};

guiImageWidget_t* wimage_new(__str filename, int imageWidth, int imageHeight)
{
	guiImageWidget_t *wimage = (guiImageWidget_t*)malloc(sizeof(guiImageWidget_t));
	wimage_init(wimage, filename, imageWidth, imageHeight);
	guiInfo("new image created");
	return wimage;
}

void wimage_init(guiImageWidget_t* wimage, __str filename, int imageWidth, int imageHeight)
{
	widget_init(&wimage->widget);
	wimage->widget.v = (guiWidget_vf_t*)&guiImage_vtable;
	
	image_setWidth(wimage->image, imageWidth);
	image_setHeight(wimage->image, imageHeight);
	image_setImage(wimage->image, filename);
	
	wimage->imageFlags = 0;
	widget_setSize((guiWidget_t *)wimage, imageWidth, imageHeight);
}

void wimage_draw(const guiImageWidget_t* wimage, guiGraphics_t* graphics)
{
	guiDebugPrint("drawing image widget");
	if (wimage->imageFlags & IF_SCALETOSIZE) {
		graph_drawImageScaled(graphics, 0, 0, image_getWidth(wimage->image), image_getHeight(wimage->image), 
			widget_getWidth(wimage), widget_getHeight(wimage), image_getImage(wimage->image));
	} else {
		graph_drawImage(graphics, 0, 0, image_getImage(wimage->image));
	}
}
