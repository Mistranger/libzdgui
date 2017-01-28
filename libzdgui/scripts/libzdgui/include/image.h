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

typedef struct guiImage {
	int imageWidth, imageHeight;
	__str filename;
} guiImage;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

#define image_getWidth(_image) ((_image).imageWidth)
#define image_getHeight(_image) ((_image).imageHeight)
#define image_setWidth(_image, _width) { (_image).imageWidth = _width; }
#define image_setHeight(_image, _height) { (_image).imageHeight = _height; }
#define image_getImage(_image) ((_image).filename)
#define image_setImage(_image, _file) { (_image).filename = _file; }


#endif // IMAGE_H_INCLUDED
