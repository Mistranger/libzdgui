#include "system.h"

#include "event.h"
#include "event/event_mouse.h"
#include "mouse.h"
#include "util/util.h"

void mouse_init(guiMouse *mouse)
{
	mouse->mouseInput = new (guiMouseInput);
	mouse->oldMouseInput = new (guiMouseInput);
	memset(mouse->mouseInput, 0, sizeof(guiMouseInput));
	memset(mouse->oldMouseInput, 0, sizeof(guiMouseInput));
	mouse->mouseEventQueue = queue_init();
	mouse->currentCursor = NULL;

	mouse->mouseSensitivity = 25;
	mouse->doubleClickDelay = 300;
	mouse->lastMousePressButton = MB_EMPTY;
	mouse->lastMouseDragButton = MB_EMPTY;
	mouse->lastMousePos = (vec2i) {0, 0};
	mouse->lastMousePressTime = 0;
	mouse->mClickCount = 0;
}

void mouse_drawCursor(guiMouse *mouse, guiGraphics *graphics)
{
	if (!mouse->currentCursor) {
		guiError("No cursor defined!");
		return;
	}
	guiDebugPrint("drawing mouse at %d %d" _C_ mouse->cursorPos.x _C_ mouse->cursorPos.y);
	graph_drawImage(graphics, mouse->cursorPos.x, mouse->cursorPos.y, image_getImage(mouse->currentCursor->image));
}

void mouse_registerCursor(guiMouse *mouse, __str image, int width, int height, int hotspotX, int hotspotY)
{
	guiCursor *cursor = new (guiCursor);
	cursor->image.filename = image;
	cursor->image.imageWidth = width;
	cursor->image.imageHeight = height;
	cursor->hotspot.x = hotspotX;
	cursor->hotspot.y = hotspotY;
	if (!mouse->currentCursor) {
		mouse->currentCursor = cursor;
	}
	guiInfo("Registered new cursor");
}

void mouse_changeCursor(guiMouse *mouse, guiCursor *newCursor)
{
	if (!newCursor) {
		guiError("No cursor defined!");
	}
	mouse->currentCursor = newCursor;
}

void mouse_grabMouseInput(guiMouse *mouse)
{
	ACS_SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
}

void mouse_releaseMouseInput(guiMouse *mouse)
{
	ACS_SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
}

void mouse_getInput(guiMouse *mouse, guiGraphics *graphics)
{
	*mouse->oldMouseInput = *mouse->mouseInput;

	int dx = ACS_GetPlayerInput(-1, INPUT_YAW);
	int dy = ACS_GetPlayerInput(-1, INPUT_PITCH);
	fixed aspectRatio = (fixed)graph_getScreenWidth(graphics)/graph_getScreenHeight(graphics);
	mouse->mouseInput->pos.x += -dx	/ (mouse_getSensitivity(mouse) * aspectRatio);
	mouse->mouseInput->pos.y += -dy	/ mouse_getSensitivity(mouse);
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
		guiMouseEvent *event = mouseEvent_new(NULL, &mouse->mouseInput->pos, MB_EMPTY, ME_MOVED);
		queue_push(mouse->mouseEventQueue, event);
	}

	if (mouse->mouseInput->button != mouse->oldMouseInput->button) {
		guiMouseEvent *event = mouseEvent_new(NULL, &mouse->mouseInput->pos, MB_EMPTY, ME_MOVED);
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
