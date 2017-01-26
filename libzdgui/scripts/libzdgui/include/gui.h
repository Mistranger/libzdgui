#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "graphics.h"
#include "event.h"
#include "event/event_mouse.h"
#include "focusmanager.h"
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
	 guiFocusManager_t *focusManager;

	 list_t *allWidgets;
	 list_t *widgetsUnderMouse;
	 int lastMouseDragButton;
} guiGUI_t;

// Constructor
void gui_init(guiGUI_t *gui);
void gui_destroy(guiGUI_t *gui);

guiWidget_t* gui_getTop(guiGUI_t *gui);
void gui_setTop(guiGUI_t *gui, guiWidget_t* newTop);
guiGraphics_t* gui_getGraphics(guiGUI_t *gui);
void gui_setGraphics(guiGUI_t *gui, guiGraphics_t* newGraphics);
guiInput_t* gui_getInput(const guiGUI_t *gui);
void gui_setInput(guiGUI_t *gui, guiInput_t* newInput);
guiMouse_t* gui_getMouse(const guiGUI_t* gui);
void gui_setMouse(guiGUI_t* gui, guiMouse_t* newMouse);

// Widget access and management
guiWidget_t* gui_getWidgetAt(guiGUI_t *gui, vec2i_t *pos);
guiWidget_t* gui_getMouseEventSource(guiGUI_t *gui, vec2i_t *pos);
void gui_addWidget(guiGUI_t *gui, void* widget);
bool gui_widgetExists(guiGUI_t *gui, guiWidget_t* widget);
void gui_deleteWidget(guiGUI_t *gui, guiWidget_t* widget);

// Focus
void gui_handleModalFocusGained(guiGUI_t* gui);
void gui_handleModalFocusReleased(guiGUI_t* gui);

// Drawing and logic
void gui_draw(guiGUI_t *gui);
void gui_tick(guiGUI_t *gui);

// Event handling functions
void gui_handleMouseInput(guiGUI_t* gui);
void gui_handleMousePressed(guiGUI_t* gui, event_t *event);
void gui_handleMouseReleased(guiGUI_t* gui, event_t *event);
void gui_distributeEvent(guiGUI_t* gui, guiWidget_t* source, event_t *event);
void gui_distributeMouseEvent(guiGUI_t* gui, guiWidget_t* widget, mouseEvent_t *event);

#endif // GUI_H_INCLUDED
