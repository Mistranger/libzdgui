#include "libzdgui.h"

#include "gui.h"

#include "graphics.h"
#include "widget.h"
#include "util/util.h"

void gui_init(guiGUI_t* gui)
{
	gui->top = NULL;
	gui->graphics = NULL;
}
guiWidget_t* gui_getTop(guiGUI_t* gui)
{
	return gui->top;
}


void gui_setTop(guiGUI_t* gui, guiWidget_t* newTop)
{
	gui->top = newTop;
}

guiGraphics_t* gui_getGraphics(guiGUI_t* gui)
{
	return gui->graphics;
}

void gui_setGraphics(guiGUI_t* gui, guiGraphics_t* newGraphics)
{
	gui->graphics = newGraphics;
}

void gui_draw(guiGUI_t* gui)
{
	Assert(gui->top);
	Assert(gui->graphics);
	
	if (!(gui->top->flags & WF_VISIBLE)) {
		return;
	}
	
	graph_pushClipArea(gui->graphics, gui->top->dim);
	gui->top->guiWidget_draw(gui->top, gui->graphics);
	graph_popClipArea(gui->graphics);
}


void gui_tick(guiGUI_t* gui)
{
	if (!gui->top) {
		// FIXME error
	}
	if (gui->input) {
		gui_handleInput(gui);
	}
	gui->top->guiWidget_tick(gui->top);
}

void gui_setInput(guiGUI_t* gui, guiInput_t* newInput)
{
	gui->input = newInput;
}

guiInput_t* gui_getInput(const guiGUI_t* gui)
{
	return gui->input;
}

static void gui_handleInput(guiGUI_t* gui)
{
	if (!gui->input) {
		// FIXME error
	}
	
	while (queue_size(gui->input->mouseEventQueue) > 0) {
		mouseEvent_t *event = (mouseEvent_t*)queue_front(gui->input->mouseEventQueue);
		
		switch (event->type) {
			case PRESSED: {
				gui_handleMousePressed(gui, event);
			}
			break;
			case RELEASED: {
				gui_handleMouseReleased(gui, event);
			}
			break;
		}
		queue_pop(gui->input->mouseEventQueue);
	}
}

guiWidget_t* gui_getWidgetAt(guiGUI_t* gui, int x, int y)
{
	guiWidget_t* parent = gui->top;
	guiWidget_t* child = gui->top;

	while (child != NULL) {
		guiWidget_t *swap = child;
		int parentX, parentY;
		widget_getAbsolutePosition(parent, &parentX, &parentY);
		child = parent->guiWidget_getWidgetAt(parent, x - parentX, y - parentY);
		parent = swap;
	}

	return parent;
}

static void gui_handleMousePressed(guiGUI_t* gui, mouseEvent_t* event)
{
	
}

static void gui_handleMouseReleased(guiGUI_t* gui, mouseEvent_t* event)
{
	
}

guiWidget_t* getMouseEventSource(guiGUI_t* gui, int x, int y)
{
	guiWidget_t* widget = gui_getWidgetAt(gui, x, y);
}
