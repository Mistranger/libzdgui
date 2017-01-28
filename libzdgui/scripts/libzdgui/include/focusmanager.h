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

typedef struct focusManager_s {
	vector_t *widgets;
	guiWidget_t *focusedWidget;
	guiWidget_t *draggedWidget;
	guiWidget_t *lastPressed;
	guiWidget_t *lastWithMouse;
	guiWidget_t *lastWithModalFocus;
	guiWidget_t *lastWithModalMouseInputFocus;
	guiWidget_t *modalFocusedWidget;
	guiWidget_t *modalMouseInputFocusedWidget;
} guiFocusManager_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

guiFocusManager_t *focusManager_new();
void focusManager_delete(guiFocusManager_t *focusManager);

void focus_requestFocus(guiFocusManager_t *focusManager, guiWidget_t *widget);
void focus_requestModalFocus(guiFocusManager_t *focusManager, guiWidget_t *widget);
void focus_requestModalMouseInputFocus(guiFocusManager_t *focusManager, guiWidget_t *widget);
void focus_releaseModalFocus(guiFocusManager_t *focusManager, guiWidget_t *widget);
void focus_releaseModalMouseInputFocus(guiFocusManager_t *focusManager, guiWidget_t *widget);
void focus_addToManager(guiFocusManager_t *focusManager, guiWidget_t *widget);
void focus_removeFromManager(guiFocusManager_t *focusManager, guiWidget_t *widget);
void focus_focusNone(guiFocusManager_t *focusManager);

#define focus_getLastPressed(_focus) (((guiFocusManager_t*)_focus)->lastPressed)
#define focus_setLastPressed(_focus, _widget) { ((guiFocusManager_t*)_focus)->lastPressed = _widget; }
#define focus_getLastWithModalFocus(_focus) (((guiFocusManager_t*)_focus)->lastWithModalFocus)
#define focus_setLastWithModalFocus(_focus, _widget) { ((guiFocusManager_t*)_focus)->lastWithModalFocus = _widget; }
#define focus_getLastWithModalMouseInputFocus(_focus) (((guiFocusManager_t*)_focus)->lastWithModalMouseInputFocus)
#define focus_setLastWithModalMouseInputFocus(_focus, _widget) { ((guiFocusManager_t*)_focus)->lastWithModalMouseInputFocus = _widget; }
#define focus_getWithMouse(_focus) (((guiFocusManager_t*)_focus)->lastWithMouse)
#define focus_setWithMouse(_focus, _widget) { ((guiFocusManager_t*)_focus)->lastWithMouse = _widget; }
#define focus_getDragged(_focus) (((guiFocusManager_t*)_focus)->draggedWidget)
#define focus_setDragged(_focus, _widget) { ((guiFocusManager_t*)_focus)->draggedWidget = _widget; }
#define focus_getFocused(_focus) (((guiFocusManager_t*)_focus)->focusedWidget)
#define focus_getModalFocused(_focus) (((guiFocusManager_t*)_focus)->modalFocusedWidget)
#define focus_getModalMouseInputFocused(_focus) (((guiFocusManager_t*)_focus)->modalMouseInputFocusedWidget)
#define focus_isFocused(_focus, _widget) (((guiFocusManager_t*)_focus)->focusedWidget == _widget)


#endif // FOCUSMANAGER_H_INCLUDED
