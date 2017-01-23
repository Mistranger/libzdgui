#ifndef DIMENSION_H_INCLUDED
#define DIMENSION_H_INCLUDED

// Generic rectangle

typedef struct vec2i_s {
	int x, y;
} vec2i_t;

#define vec_add(v1, v2) { v1.x += v2.x; v1.y += v2.y; }
#define vec_sub(v1, v2) { v1.x -= v2.x; v1.y -= v2.y; }
#define vec_add2(v1, v2, v3) { v1.x = v2.x + v3.x; v1.y = v2.y + v3.y; }
#define vec_sub2(v1, v2, v3) { v1.x = v2.x - v3.x; v1.y = v2.y - v3.y; }

typedef struct guiRectangle_s {
    vec2i_t pos;
    int width;
    int height;
} guiRectangle_t;

// Rectangle for graphic clipping purposes
typedef struct guiClipRectangle_s
{
	guiRectangle_t rect;
    vec2i_t offset; // Actual drawing offset from top left
} guiClipRectangle_t;

bool rect_isPointInRect(const guiRectangle_t *rect, vec2i_t pos);
bool rect_intersect(guiRectangle_t *rect, guiRectangle_t *other);

#endif // DIMENSION_H_INCLUDED
