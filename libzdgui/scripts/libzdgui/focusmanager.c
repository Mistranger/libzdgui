#include "system.h"

#include "focusmanager.h"

#include "event/event_focus.h"

guiFocusManager_t* focusManager_new()
{
	guiFocusManager_t *focusManager = new(guiFocusManager_t);
	focusManager->widgets = vector_new(sizeof(guiWidget_t*));
	focusManager->focusedWidget = NULL;
	focusManager->draggedWidget = NULL;
	focusManager->lastPressed = NULL;
	focusManager->lastWithMouse = NULL;
	focusManager->lastWithModalFocus = NULL;
	focusManager->lastWithModalMouseInputFocus = NULL;
	focusManager->modalFocusedWidget = NULL;
	focusManager->modalMouseInputFocusedWidget = NULL;
	return focusManager;
}

void focusManager_delete(guiFocusManager_t *focusManager)
{
	// FIXME remove vector
}

void focus_requestFocus(guiFocusManager_t *focusManager, guiWidget_t *widget)
{
	if (!widget || widget == focusManager->focusedWidget) {
		return;
	}
	int focused = -1;
	for (size_t i = 0; i < vector_size(focusManager->widgets); ++i)	{
		guiWidget_t *w = vector_get(focusManager->widgets, i, guiWidget_t*);
		
		if (vector_get(focusManager->widgets, i, guiWidget_t*) == widget) {
			focused = i;
			break;
		}
	}
	
	if (focused < 0) {
		guiError("Trying to focus a none existing widget.");
		return;
	}

	guiWidget_t *oldFocused = focus_getFocused(focusManager);

	if (oldFocused != widget) {
		focusManager->focusedWidget = vector_get(focusManager->widgets, focused, guiWidget_t*);

		if (oldFocused != NULL)	{
			focusEvent_t *event = focusEvent_new(oldFocused, FE_FOCUSLOST);
			widget_handleEvent(oldFocused, (event_t*)event);
		}

		focusEvent_t *event = focusEvent_new((void*)focusManager->focusedWidget, FE_FOCUSGAINED);
		widget_handleEvent((void*)focusManager->focusedWidget, (event_t*)event);
		
	}
}

void focus_requestModalFocus(guiFocusManager_t *focusManager, guiWidget_t *widget)
{
	if (focusManager->modalFocusedWidget && focusManager->modalFocusedWidget != widget) {
		guiWarning("Another widget already has modal focus.");
		return;
	}
	focusManager->modalFocusedWidget = widget;
	if (focusManager->modalFocusedWidget && !widget_hasModalFocus(focusManager->focusedWidget)) {
		focus_focusNone(focusManager);
	}
}

void focus_requestModalMouseInputFocus(guiFocusManager_t *focusManager, guiWidget_t *widget)
{
	if (focusManager->modalMouseInputFocusedWidget != NULL
		&& focusManager->modalMouseInputFocusedWidget != widget) {
			guiWarning("Another widget already has modal focus.");
			return;
	}

	focusManager->modalMouseInputFocusedWidget = widget;
}

void focus_releaseModalFocus(guiFocusManager_t *focusManager, guiWidget_t *widget)
{
	if (focusManager->modalFocusedWidget == widget) {
		focusManager->modalFocusedWidget = NULL;
	}
}

void focus_releaseModalMouseInputFocus(guiFocusManager_t *focusManager, guiWidget_t *widget)
{
	if (focusManager->modalMouseInputFocusedWidget == widget) {
		focusManager->modalMouseInputFocusedWidget = NULL;
	}
}

void focus_focusNone(guiFocusManager_t *focusManager)
{
	if (focusManager->focusedWidget != NULL) {
		guiWidget_t* focused = focusManager->focusedWidget;
		focusManager->focusedWidget = NULL;

		focusEvent_t *event = focusEvent_new(focused, FE_FOCUSLOST);
		widget_handleEvent(focused, (event_t*)event);
	}
}

void focus_addToManager(guiFocusManager_t *focusManager, guiWidget_t *widget)
{
	vector_push_back(focusManager->widgets, &widget);
}

void focus_removeFromManager(guiFocusManager_t *focusManager, guiWidget_t *widget)
{
	if (focus_isFocused(focusManager, widget)) {
		focusManager->focusedWidget = NULL;
	}

	guiWidget_t *w;
	for (size_t i = 0; i < vector_size(focusManager->widgets); ++i) {
		if (vector_get(focusManager->widgets, i, guiWidget_t*) == widget) {
			vector_erase(focusManager->widgets, i);
			break;
		}
	}

	if (focusManager->draggedWidget == widget) {
		focusManager->draggedWidget = NULL;
		return;
	}
	if (focusManager->lastWithMouse == widget) {
		focusManager->lastWithMouse = NULL;
		return;
	}
}
