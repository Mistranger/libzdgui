#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "graphics.h"
#include "event.h"
#include "event/event_mouse.h"
#include "focusmanager.h"
#include "input.h"
#include "mouse.h"
#include "widget.h"

typedef struct guiGUI {
	// The top widget
	guiWidget *top;
	guiGraphics *graphics;
	guiInput *input;
	guiMouse *mouse;
	guiFocusManager *focusManager;

	list_t *allWidgets;
	list_t *widgetsUnderMouse;
} guiGUI;

// Constructor
void gui_init(guiGUI *gui, int screenWidth, int screenHeight);
void gui_destroy(guiGUI *gui);

guiWidget *gui_getTop(guiGUI *gui);
void gui_setTop(guiGUI *gui, guiWidget *newTop);
#define gui_getTop(_gui) (((guiGUI*)_gui)->top)
#define gui_getMouse(_gui) (((guiGUI*)_gui)->mouse)
#define gui_getInput(_gui) (((guiGUI*)_gui)->input)
#define gui_getGraphics(_gui) (((guiGUI*)_gui)->graphics)

// Widget access and management
guiWidget *gui_getWidgetAt(guiGUI *gui, vec2i *pos);
guiWidget *gui_getMouseEventSource(guiGUI *gui, vec2i *pos);
void gui_addWidget(guiGUI *gui, void *widget);
bool gui_widgetExists(guiGUI *gui, guiWidget *widget);
void gui_deleteWidget(guiGUI *gui, guiWidget *widget);

// Focus
void gui_handleModalFocusGained(guiGUI *gui);
void gui_handleModalFocusReleased(guiGUI *gui);

// Drawing and logic
void gui_draw(guiGUI *gui);
void gui_tick(guiGUI *gui);

// Event handling functions
void gui_handleMouseInput(guiGUI *gui);
void gui_handleMousePressed(guiGUI *gui, guiEvent *event);
void gui_handleMouseReleased(guiGUI *gui, guiEvent *event);
void gui_distributeEvent(guiGUI *gui, guiWidget *source, guiEvent *event, bool force, bool toSourceOnly);

#endif // GUI_H_INCLUDED
