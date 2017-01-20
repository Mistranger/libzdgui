#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "util/queue.h"
#include "event.h"
#include "graphics.h"

typedef struct inputMouse_s
{
	int x, y;
	enum mouseButtons button;
} inputMouse_t;


typedef struct guiInput_s {
	queue_t *mouseEventQueue;
	inputMouse_t *mouseInput, *oldMouseInput;
	
} guiInput_t;

void input_init(guiInput_t *input);
void input_getInput(guiInput_t *input, guiGraphics_t *graphics);

#endif // INPUT_H_INCLUDED
