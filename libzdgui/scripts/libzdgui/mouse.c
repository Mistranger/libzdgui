#include "system.h"

#include "event.h"
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
}

void mouse_drawCursor(guiMouse_t* mouse, guiGraphics_t* graphics)
{
	if (!mouse->currentCursor) {
		
	}
	graph_drawImage(graphics, mouse->cursorPos.x, mouse->cursorPos.y, image_getImage(mouse->currentCursor->image));
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
	mouse->oldMouseInput = mouse->mouseInput;
	
	int dx = ACS_GetPlayerInput(-1, INPUT_YAW);
	int dy = ACS_GetPlayerInput(-1, INPUT_PITCH);
	mouse->mouseInput->pos.x += -dx	/ 35;
	mouse->mouseInput->pos.y += -dy	/ 25;
	
	clamp(mouse->mouseInput->pos.x, 0, graph_getScreenWidth(graphics));
	clamp(mouse->mouseInput->pos.y, 0, graph_getScreenHeight(graphics));
	
	int buttons = ACS_GetPlayerInput(-1, INPUT_BUTTONS);
	
	if (buttons & BT_ATTACK) {
		mouse->mouseInput->button |= LEFT;
	}
	
	if (buttons & BT_ALTATTACK) {
		mouse->mouseInput->button |= RIGHT;
	}
	
	if ((mouse->mouseInput->button & LEFT) && !(mouse->oldMouseInput->button & LEFT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *lpressed = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		lpressed->pos = mouse->mouseInput->pos;
		lpressed->type = ME_PRESSED;
		lpressed->button = LEFT;
		event->events.mouse = lpressed;
		queue_push(mouse->mouseEventQueue, event);
	}
	if (!(mouse->mouseInput->button & LEFT) && (mouse->oldMouseInput->button & LEFT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *lreleased = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		lreleased->pos = mouse->mouseInput->pos;
		lreleased->type = ME_RELEASED;
		lreleased->button = LEFT;
		event->events.mouse = lreleased;
		queue_push(mouse->mouseEventQueue, event);
	}
	if ((mouse->mouseInput->button & RIGHT) && !(mouse->oldMouseInput->button & RIGHT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *rpressed = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		rpressed->pos = mouse->mouseInput->pos;
		rpressed->type = ME_PRESSED;
		rpressed->button = RIGHT;
		event->events.mouse = rpressed;
		queue_push(mouse->mouseEventQueue, event);
	}
	if (!(mouse->mouseInput->button & RIGHT) && (mouse->oldMouseInput->button & RIGHT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *rreleased = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		rreleased->pos = mouse->mouseInput->pos;
		rreleased->type = ME_RELEASED;
		rreleased->button = RIGHT;
		event->events.mouse = rreleased;
		queue_push(mouse->mouseEventQueue, event);
	}
	
}
