#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "graphics.h"
#include "event.h"
#include "input.h"
#include "mouse.h"
#include "widget.h"

typedef struct guiGUI_s
{
	// The top widget
	 guiWidget_t *top;
	 guiGraphics_t *graphics;
	 guiInput_t *input;
	 guiMouse_t *mouse;
} guiGUI_t;

// Constructor
void gui_init(guiGUI_t *gui);

guiWidget_t* gui_getTop(guiGUI_t *gui);
void gui_setTop(guiGUI_t *gui, guiWidget_t* newTop);
guiGraphics_t* gui_getGraphics(guiGUI_t *gui);
void gui_setGraphics(guiGUI_t *gui, guiGraphics_t* newGraphics);
guiInput_t* gui_getInput(const guiGUI_t *gui);
void gui_setInput(guiGUI_t *gui, guiInput_t* newInput);
guiMouse_t* gui_getMouse(const guiGUI_t* gui);
void gui_setMouse(guiGUI_t* gui, guiMouse_t* newMouse);

// Widget access
guiWidget_t* gui_getWidgetAt(guiGUI_t *gui, vec2i_t pos);
guiWidget_t* gui_getMouseEventSource(guiGUI_t *gui, vec2i_t pos);
bool gui_isWidgetExisting(guiGUI_t *gui, const guiWidget_t* widget);

// Drawing and logic
void gui_draw(guiGUI_t *gui);
void gui_tick(guiGUI_t *gui);

// Event handling functions
static void gui_handleMouseInput(guiGUI_t* gui);
static void gui_handleMousePressed(guiGUI_t* gui, event_t *event);
static void gui_handleMouseReleased(guiGUI_t* gui, event_t *event);
static void gui_distributeEvent(guiGUI_t* gui, guiWidget_t* source, event_t *event);
static void gui_distributeMouseEvent(guiGUI_t* gui, guiWidget_t* widget, mouseEvent_t *event);

#endif // GUI_H_INCLUDED
