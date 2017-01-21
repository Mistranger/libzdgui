#ifndef DIMENSION_H_INCLUDED
#define DIMENSION_H_INCLUDED

// Generic rectangle
typedef struct guiRectangle_s
{
    int x;
    int y;
    int width;
    int height;
} guiRectangle_t;

// Rectangle for graphic clipping purposes
typedef struct guiClipRectangle_s
{
	guiRectangle_t rect;
    int xOffset; // Actual drawing offset from top left
    int yOffset;
} guiClipRectangle_t;

bool rect_isPointInRect(const guiRectangle_t *rect, int x, int y);

#endif // DIMENSION_H_INCLUDED
