#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "dimension.h"
#include "graphics.h"
#include "image.h"
#include "util/list.h"
#include "util/queue.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef enum mouseButtons {
	MB_EMPTY = 0x00,
	MB_LEFT = 0x01,
	MB_RIGHT = 0x02,
	MB_MIDDLE = 0x04
} mouseButtons_t;

typedef struct guiCursor_s {
	guiImage_t image;
	vec2i_t hotspot;
} guiCursor_t;

typedef struct inputMouse_s
{
	vec2i_t pos;
	mouseButtons_t button;
} inputMouse_t;

typedef struct guiMouse_s {
	list_t *cursors;
	guiCursor_t *currentCursor;
	vec2i_t cursorPos;
	int mouseSensitivity;
	int doubleClickDelay;

	int mClickCount;
	int lastMousePressTime;
	int lastMousePressButton;
	
	queue_t *mouseEventQueue;
	inputMouse_t *mouseInput, *oldMouseInput;
} guiMouse_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

void mouse_init(guiMouse_t* input);

void mouse_drawCursor(guiMouse_t *mouse, guiGraphics_t *graphics);
void mouse_registerCursor(guiMouse_t* mouse, __str image, int width, int height, int hotspotX, int hotspotY);
void mouse_grabMouseInput(guiMouse_t *mouse);
void mouse_releaseMouseInput(guiMouse_t *mouse);
void mouse_getInput(guiMouse_t* input, guiGraphics_t *graphics);

#endif // MOUSE_H_INCLUDED
