#include "libzdgui.h"

#include "input.h"

#include "graphics.h"
#include "util/util.h"

void input_init(guiInput_t* input)
{
	input->mouseInput = (inputMouse_t*)malloc(sizeof(inputMouse_t));
	input->oldMouseInput = (inputMouse_t*)malloc(sizeof(inputMouse_t));
	memset(input->mouseInput, 0, sizeof(inputMouse_t));
	memset(input->oldMouseInput, 0, sizeof(inputMouse_t));
}

void input_getInput(guiInput_t* input, guiGraphics_t *graphics)
{
	input->oldMouseInput = input->mouseInput;
	
	int dx = ACS_GetPlayerInput(-1, INPUT_YAW);
	int dy = ACS_GetPlayerInput(-1, INPUT_PITCH);
	input->mouseInput->x += -dx	/ 35;
	input->mouseInput->y += -dy	/ 25;
	
	clamp(input->mouseInput->x, 0, graph_getScreenWidth(graphics));
	clamp(input->mouseInput->y, 0, graph_getScreenHeight(graphics));
	
	int buttons = ACS_GetPlayerInput(-1, INPUT_BUTTONS);
	
	if (buttons & BT_ATTACK) {
		input->mouseInput->button &= LEFT;
	}
	
	if (buttons & BT_ALTATTACK) {
		input->mouseInput->button &= RIGHT;
	}
	
	if ((input->mouseInput->button & LEFT) && !(input->oldMouseInput->button & LEFT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *lpressed = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		lpressed->x = input->mouseInput->x;
		lpressed->y = input->mouseInput->y;
		lpressed->type = PRESSED;
		lpressed->button = LEFT;
		event->events.mouse = lpressed;
		queue_push(input->mouseEventQueue, event);
	}
	if (!(input->mouseInput->button & LEFT) && (input->oldMouseInput->button & LEFT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *lreleased = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		lreleased->x = input->mouseInput->x;
		lreleased->y = input->mouseInput->y;
		lreleased->type = RELEASED;
		lreleased->button = LEFT;
		event->events.mouse = lreleased;
		queue_push(input->mouseEventQueue, event);
	}
	if ((input->mouseInput->button & RIGHT) && !(input->oldMouseInput->button & RIGHT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *rpressed = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		rpressed->x = input->mouseInput->x;
		rpressed->y = input->mouseInput->y;
		rpressed->type = PRESSED;
		rpressed->button = RIGHT;
		event->events.mouse = rpressed;
		queue_push(input->mouseEventQueue, event);
	}
	if (!(input->mouseInput->button & RIGHT) && (input->oldMouseInput->button & RIGHT)) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Mouse;
		mouseEvent_t *rreleased = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
		rreleased->x = input->mouseInput->x;
		rreleased->y = input->mouseInput->y;
		rreleased->type = RELEASED;
		rreleased->button = RIGHT;
		event->events.mouse = rreleased;
		queue_push(input->mouseEventQueue, event);
	}
	
}
