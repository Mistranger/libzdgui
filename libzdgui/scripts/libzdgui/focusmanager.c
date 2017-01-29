#include "system.h"

#include "focusmanager.h"

#include "event/event_focus.h"

guiFocusManager *focusManager_new()
{
	guiFocusManager *focusManager = new (guiFocusManager);
	focusManager->widgets = vector_new(sizeof(guiWidget *));
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

void focusManager_delete(guiFocusManager *focusManager)
{
	if (focusManager) {
		vector_delete(focusManager->widgets);
		free(focusManager);
	}
}

void focus_requestFocus(guiFocusManager *focusManager, guiWidget *widget)
{
	if (!widget || widget == focusManager->focusedWidget) {
		return;
	}
	int focused = -1;
	for (size_t i = 0; i < vector_size(focusManager->widgets); ++i)	{
		if (vector_get(focusManager->widgets, i, guiWidget *) == widget) {
			focused = i;
			break;
		}
	}

	if (focused < 0) {
		guiError("Trying to focus a none existing widget.");
		return;
	}

	guiWidget *oldFocused = focus_getFocused(focusManager);

	if (oldFocused != widget) {
		focusManager->focusedWidget = vector_get(focusManager->widgets, focused, guiWidget *);

		if (oldFocused != NULL)	{
			guiFocusEvent *event = focusEvent_new(oldFocused, FE_FOCUSLOST);
			widget_handleEvent(oldFocused, (guiEvent *)event, true);
		}

		guiFocusEvent *event = focusEvent_new((void *)focusManager->focusedWidget, FE_FOCUSGAINED);
		widget_handleEvent((void *)focusManager->focusedWidget, (guiEvent *)event, true);

	}
}

void focus_requestModalFocus(guiFocusManager *focusManager, guiWidget *widget)
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

void focus_requestModalMouseInputFocus(guiFocusManager *focusManager, guiWidget *widget)
{
	if (focusManager->modalMouseInputFocusedWidget != NULL
		&& focusManager->modalMouseInputFocusedWidget != widget) {
		guiWarning("Another widget already has modal focus.");
		return;
	}

	focusManager->modalMouseInputFocusedWidget = widget;
}

void focus_releaseModalFocus(guiFocusManager *focusManager, guiWidget *widget)
{
	if (focusManager->modalFocusedWidget == widget) {
		focusManager->modalFocusedWidget = NULL;
	}
}

void focus_releaseModalMouseInputFocus(guiFocusManager *focusManager, guiWidget *widget)
{
	if (focusManager->modalMouseInputFocusedWidget == widget) {
		focusManager->modalMouseInputFocusedWidget = NULL;
	}
}

void focus_focusNone(guiFocusManager *focusManager)
{
	if (focusManager->focusedWidget != NULL) {
		guiWidget *focused = focusManager->focusedWidget;
		focusManager->focusedWidget = NULL;

		guiFocusEvent *event = focusEvent_new(focused, FE_FOCUSLOST);
		widget_handleEvent(focused, (guiEvent *)event, true);
	}
}

void focus_addToManager(guiFocusManager *focusManager, guiWidget *widget)
{
	vector_push_back(focusManager->widgets, &widget);
}

void focus_removeFromManager(guiFocusManager *focusManager, guiWidget *widget)
{
	if (focus_isFocused(focusManager, widget)) {
		focusManager->focusedWidget = NULL;
	}

	for (size_t i = 0; i < vector_size(focusManager->widgets); ++i) {
		if (vector_get(focusManager->widgets, i, guiWidget *) == widget) {
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
