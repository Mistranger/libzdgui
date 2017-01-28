#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "graphics.h"
#include "event.h"
#include "event/event_mouse.h"
#include "focusmanager.h"
#include "input.h"
#include "mouse.h"
#include "widget.h"

typedef struct guiGUI_s {
	// The top widget
	guiWidget_t *top;
	guiGraphics_t *graphics;
	guiInput_t *input;
	guiMouse_t *mouse;
	guiFocusManager_t *focusManager;

	list_t *allWidgets;
	list_t *widgetsUnderMouse;
} guiGUI_t;

// Constructor
void gui_init(guiGUI_t *gui, int screenWidth, int screenHeight);
void gui_destroy(guiGUI_t *gui);

guiWidget_t *gui_getTop(guiGUI_t *gui);
void gui_setTop(guiGUI_t *gui, guiWidget_t *newTop);
#define gui_getTop(_gui) (((guiGUI_t*)_gui)->top)
#define gui_getMouse(_gui) (((guiGUI_t*)_gui)->mouse)
#define gui_getInput(_gui) (((guiGUI_t*)_gui)->input)
#define gui_getGraphics(_gui) (((guiGUI_t*)_gui)->graphics)

// Widget access and management
guiWidget_t *gui_getWidgetAt(guiGUI_t *gui, vec2i_t *pos);
guiWidget_t *gui_getMouseEventSource(guiGUI_t *gui, vec2i_t *pos);
void gui_addWidget(guiGUI_t *gui, void *widget);
bool gui_widgetExists(guiGUI_t *gui, guiWidget_t *widget);
void gui_deleteWidget(guiGUI_t *gui, guiWidget_t *widget);

// Focus
void gui_handleModalFocusGained(guiGUI_t *gui);
void gui_handleModalFocusReleased(guiGUI_t *gui);

// Drawing and logic
void gui_draw(guiGUI_t *gui);
void gui_tick(guiGUI_t *gui);

// Event handling functions
void gui_handleMouseInput(guiGUI_t *gui);
void gui_handleMousePressed(guiGUI_t *gui, event_t *event);
void gui_handleMouseReleased(guiGUI_t *gui, event_t *event);
void gui_distributeEvent(guiGUI_t *gui, guiWidget_t *source, event_t *event);

#endif // GUI_H_INCLUDED
