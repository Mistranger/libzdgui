#ifndef FOCUSMANAGER_H_INCLUDED
#define FOCUSMANAGER_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "util/vector.h"
#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

typedef struct guiFocusManager {
	vector_t *widgets;
	guiWidget *focusedWidget;
	guiWidget *draggedWidget;
	guiWidget *lastPressed;
	guiWidget *lastWithMouse;
	guiWidget *lastWithModalFocus;
	guiWidget *lastWithModalMouseInputFocus;
	guiWidget *modalFocusedWidget;
	guiWidget *modalMouseInputFocusedWidget;
} guiFocusManager;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

guiFocusManager *focusManager_new();
void focusManager_delete(guiFocusManager *focusManager);

void focus_requestFocus(guiFocusManager *focusManager, guiWidget *widget);
void focus_requestModalFocus(guiFocusManager *focusManager, guiWidget *widget);
void focus_requestModalMouseInputFocus(guiFocusManager *focusManager, guiWidget *widget);
void focus_releaseModalFocus(guiFocusManager *focusManager, guiWidget *widget);
void focus_releaseModalMouseInputFocus(guiFocusManager *focusManager, guiWidget *widget);
void focus_addToManager(guiFocusManager *focusManager, guiWidget *widget);
void focus_removeFromManager(guiFocusManager *focusManager, guiWidget *widget);
void focus_focusNone(guiFocusManager *focusManager);

#define focus_getLastPressed(_focus) (((guiFocusManager*)_focus)->lastPressed)
#define focus_setLastPressed(_focus, _widget) { ((guiFocusManager*)_focus)->lastPressed = _widget; }
#define focus_getLastWithModalFocus(_focus) (((guiFocusManager*)_focus)->lastWithModalFocus)
#define focus_setLastWithModalFocus(_focus, _widget) { ((guiFocusManager*)_focus)->lastWithModalFocus = _widget; }
#define focus_getLastWithModalMouseInputFocus(_focus) (((guiFocusManager*)_focus)->lastWithModalMouseInputFocus)
#define focus_setLastWithModalMouseInputFocus(_focus, _widget) { ((guiFocusManager*)_focus)->lastWithModalMouseInputFocus = _widget; }
#define focus_getWithMouse(_focus) (((guiFocusManager*)_focus)->lastWithMouse)
#define focus_setWithMouse(_focus, _widget) { ((guiFocusManager*)_focus)->lastWithMouse = _widget; }
#define focus_getDragged(_focus) (((guiFocusManager*)_focus)->draggedWidget)
#define focus_setDragged(_focus, _widget) { ((guiFocusManager*)_focus)->draggedWidget = _widget; }
#define focus_getFocused(_focus) (((guiFocusManager*)_focus)->focusedWidget)
#define focus_getModalFocused(_focus) (((guiFocusManager*)_focus)->modalFocusedWidget)
#define focus_getModalMouseInputFocused(_focus) (((guiFocusManager*)_focus)->modalMouseInputFocusedWidget)
#define focus_isFocused(_focus, _widget) (((guiFocusManager*)_focus)->focusedWidget == _widget)


#endif // FOCUSMANAGER_H_INCLUDED
