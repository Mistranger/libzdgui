#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef struct guiImage_s {
	int imageWidth, imageHeight;
	__str filename;
} guiImage_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

#define image_getWidth(image) ((image).imageWidth)
#define image_getHeight(image) ((image).imageHeight)
#define image_setWidth(image, width) { (image).imageWidth = width; }
#define image_setHeight(image, height) { (image).imageHeight = height; }
#define image_getImage(image) ((image).filename)
#define image_setImage(image, file) { (image).filename = file; }


#endif // IMAGE_H_INCLUDED
