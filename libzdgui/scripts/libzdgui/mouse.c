#include "system.h"

#include "event.h"
#include "event/event_mouse.h"
#include "mouse.h"
#include "util/util.h"


void mouse_init(guiMouse_t* mouse)
{
	mouse->mouseInput = new(inputMouse_t);
	mouse->oldMouseInput = new(inputMouse_t);
	memset(mouse->mouseInput, 0, sizeof(inputMouse_t));
	memset(mouse->oldMouseInput, 0, sizeof(inputMouse_t));
	mouse->mouseEventQueue = queue_init();
	mouse->currentCursor = NULL;
	mouse->cursors = list_new();

	mouse->lastMousePressButton = MB_EMPTY;
	mouse->lastMousePressTime = 0;
	mouse->mClickCount = 0;
}

void mouse_drawCursor(guiMouse_t* mouse, guiGraphics_t* graphics)
{
	if (!mouse->currentCursor) {
		guiError("No cursor defined!");
		return;
	}
	guiDebugPrint("drawing mouse at %d %d" _C_ mouse->cursorPos.x _C_ mouse->cursorPos.y);
	graph_drawImage(graphics, mouse->cursorPos.x, mouse->cursorPos.y, image_getImage(mouse->currentCursor->image));
}

void mouse_registerCursor(guiMouse_t* mouse, __str image, int width, int height, int hotspotX, int hotspotY)
{
	guiCursor_t *cursor = new(guiCursor_t);
	cursor->image.filename = image;
	cursor->image.imageWidth = width;
	cursor->image.imageHeight = height;
	cursor->hotspot.x = hotspotX;
	cursor->hotspot.y = hotspotY;
	list_push_back(mouse->cursors, cursor);
	if (!mouse->currentCursor) {
		mouse->currentCursor = cursor;
	}
	guiInfo("Registered new cursor");
}

void mouse_grabMouseInput(guiMouse_t* mouse)
{
	ACS_SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
}

void mouse_releaseMouseInput(guiMouse_t* mouse)
{
	ACS_SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
}


void mouse_getInput(guiMouse_t* mouse, guiGraphics_t *graphics)
{
	*mouse->oldMouseInput = *mouse->mouseInput;
	
	int dx = ACS_GetPlayerInput(-1, INPUT_YAW);
	int dy = ACS_GetPlayerInput(-1, INPUT_PITCH);
	mouse->mouseInput->pos.x += -dx	/ 35;
	mouse->mouseInput->pos.y += -dy	/ 25;
	clamp(mouse->mouseInput->pos.x, 0, graph_getScreenWidth(graphics));
	clamp(mouse->mouseInput->pos.y, 0, graph_getScreenHeight(graphics));
	mouse->cursorPos.x = mouse->mouseInput->pos.x;
	mouse->cursorPos.y = mouse->mouseInput->pos.y;	
	
	int buttons = ACS_GetPlayerInput(-1, INPUT_BUTTONS);
	
	mouse->mouseInput->button = 0;
	if (buttons & BT_ATTACK) {
		mouse->mouseInput->button |= MB_LEFT;
	}
	
	if (buttons & BT_ALTATTACK) {
		mouse->mouseInput->button |= MB_RIGHT;
	}
	
	if (mouse->mouseInput->pos.x != mouse->oldMouseInput->pos.x
		|| mouse->mouseInput->pos.y != mouse->oldMouseInput->pos.y) {
			mouseEvent_t *event = mouseEvent_new(NULL, &mouse->mouseInput->pos, MB_EMPTY, ME_MOVED);
			queue_push(mouse->mouseEventQueue, event);
	}
	
	if (mouse->mouseInput->button != mouse->oldMouseInput->button) {
		mouseEvent_t *event = mouseEvent_new(NULL, &mouse->mouseInput->pos, MB_EMPTY, ME_MOVED);
		if ((mouse->mouseInput->button & MB_LEFT) && !(mouse->oldMouseInput->button & MB_LEFT)) {
			event->type = ME_PRESSED;
			event->button = MB_LEFT;
		} else if (!(mouse->mouseInput->button & MB_LEFT) && (mouse->oldMouseInput->button & MB_LEFT)) {
			event->type = ME_RELEASED;
			event->button = MB_LEFT;
		} else if ((mouse->mouseInput->button & MB_RIGHT) && !(mouse->oldMouseInput->button & MB_RIGHT)) {
			event->type = ME_PRESSED;
			event->button = MB_RIGHT;
		} else if (!(mouse->mouseInput->button & MB_RIGHT) && (mouse->oldMouseInput->button & MB_RIGHT)) {
			event->type = ME_RELEASED;
			event->button = MB_RIGHT;
		} else {
			guiError("Wrong mouse buttons!");
		}
		queue_push(mouse->mouseEventQueue, event);
	}	
}
