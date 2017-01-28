#include "system.h"

#include "widgets/imagewidget.h"

guiImageWidget_vf_t guiImage_vtable = {
	wimage_typename,
	widget_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	wimage_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager
};

const char *ImageWidgetType = "ImageWidget";

const char *wimage_typename(guiImageWidget *widget)
{
	return ImageWidgetType;
}

guiImageWidget *wimage_new(guiGUI *gui, __str filename, int imageWidth, int imageHeight)
{
	guiImageWidget *wimage = new (guiImageWidget);
	wimage_init(wimage, filename, imageWidth, imageHeight);
	gui_addWidget(gui, wimage);
	guiInfo("new image created");
	return wimage;
}

void wimage_init(guiImageWidget *wimage, __str filename, int imageWidth, int imageHeight)
{
	widget_init(&wimage->widget);
	wimage->widget.v = (guiWidget_vf_t *)&guiImage_vtable;

	image_setWidth(wimage->image, imageWidth);
	image_setHeight(wimage->image, imageHeight);
	image_setImage(wimage->image, filename);

	wimage->imageFlags = 0;
	widget_setSize((guiWidget *)wimage, imageWidth, imageHeight);
}

void wimage_draw(const guiImageWidget *wimage, guiGraphics *graphics)
{
	guiDebugPrint("drawing image widget");
	if (wimage->imageFlags & IF_SCALETOSIZE) {
		graph_drawImageScaled(graphics, 0, 0, image_getWidth(wimage->image), image_getHeight(wimage->image),
							  widget_getWidth(wimage), widget_getHeight(wimage), image_getImage(wimage->image));
	} else {
		graph_drawImage(graphics, 0, 0, image_getImage(wimage->image));
	}
}

