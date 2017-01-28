#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "dimension.h"
#include "graphics.h"
#include "image.h"
#include "util/list.h"
#include "util/queue.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

/// Enum holding all possible mouse buttons
typedef enum mouseButtons {
	MB_EMPTY = 0x00,   /// No button is pressed
	MB_LEFT = 0x01,    /// Left button is pressed
	MB_RIGHT = 0x02,   /// Right button is pressed
	MB_MIDDLE = 0x04   /// Middle button is pressed
} mouseButtons_t;

/// Cursor
typedef struct guiCursor_s {
	guiImage_t image;  /// Image used for cursor
	vec2i_t hotspot;   /// Click position in the cursor
} guiCursor_t;

/// Structure with mouse input data
typedef struct inputMouse_s {
	vec2i_t pos;
	mouseButtons_t button;
} inputMouse_t;

typedef struct guiMouse_s {
	list_t *cursors;                           /// All cursors that are available
	guiCursor_t *currentCursor;                /// Current cursor in use
	vec2i_t cursorPos;                         /// Cursor position
	int mouseSensitivity;                      /// Overall mouse move sensitivity
	int doubleClickDelay;                      /// Delay for double clicks

	int mClickCount;                           /// Internal: used for double click detection
	int lastMousePressTime;                    /// Internal: timestamp of mouse last press
	int lastMousePressButton;                  /// Internal: last pressed button
	int lastMouseDragButton;                   /// Internal: last button used for dragging
	vec2i_t lastMousePos;                      /// Internal: last mouse position

	queue_t *mouseEventQueue;                  /// All mouse generated input first goes here
	inputMouse_t *mouseInput, *oldMouseInput;  /// Mouse input data for current and last tic
} guiMouse_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

/**
 * @brief Initializes mouse subsystem
 *
 * Prepares mouse subsystem for use (sets default values for variables)
 *
 * @param input Mouse struct to init
 */
void mouse_init(guiMouse_t *input);

/**
 * @brief Draws the current cursor
 *
 * Put full description here
 *
 * @param mouse Mouse subsystem
 * @param graphics Graphics subsystem
 */
void mouse_drawCursor(guiMouse_t *mouse, guiGraphics_t *graphics);

/**
 * @brief Registers new cursor in system
 *
 * Put full description here
 *
 * @param mouse Mouse subsystem
 * @param image Image file used for cursor (8 characters max, ACS HudMessage limit)
 * @param width Image width
 * @param height Image height
 * @param hotspotX Cursor X click position
 * @param hotspotY Cursor Y click position
 */
void mouse_registerCursor(guiMouse_t *mouse, __str image, int width, int height, int hotspotX, int hotspotY);

/**
 * @brief Put brief description here
 *
 * Put full description here
 *
 * @param mouse Put argument desc here
 * @return Put return information here
 */
void mouse_grabMouseInput(guiMouse_t *mouse);
void mouse_releaseMouseInput(guiMouse_t *mouse);
void mouse_getInput(guiMouse_t *input, guiGraphics_t *graphics);
#define mouse_getLastPressButton(_mouse) (((guiMouse_t*)_mouse)->lastMousePressButton)
#define mouse_setLastPressButton(_mouse, _button) { ((guiMouse_t*)_mouse)->lastMousePressButton = _button; }
#define mouse_getLastDragButton(_mouse) (((guiMouse_t*)_mouse)->lastMouseDragButton)
#define mouse_setLastDragButton(_mouse, _button) { ((guiMouse_t*)_mouse)->lastMouseDragButton = _button; }

#endif // MOUSE_H_INCLUDED
