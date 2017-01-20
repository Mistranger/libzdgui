#ifndef DIMENSION_H_INCLUDED
#define DIMENSION_H_INCLUDED

typedef struct guiRectangle_s
{
    int x;
    int y;
    int width;
    int height;
} guiRectangle_t;

bool rect_isPointInRect(const guiRectangle_t *rect, int x, int y);


#endif // DIMENSION_H_INCLUDED
