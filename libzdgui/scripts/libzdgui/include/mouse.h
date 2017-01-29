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
typedef enum guiMouseButtons {
	MB_EMPTY = 0x00,   /// No button is pressed
	MB_LEFT = 0x01,    /// Left button is pressed
	MB_RIGHT = 0x02,   /// Right button is pressed
	MB_MIDDLE = 0x04   /// Middle button is pressed
} guiMouseButtons;

/// Cursor
typedef struct guiCursor {
	guiImage image;  /// Image used for cursor
	vec2i hotspot;   /// Click position in the cursor
} guiCursor;

/// Structure with mouse input data
typedef struct guiMouseInput {
	vec2i pos;
	guiMouseButtons button;
} guiMouseInput;

typedef struct guiMouse {
	list_t *cursors;                           /// All cursors that are available
	guiCursor *currentCursor;                  /// Current cursor in use
	vec2i cursorPos;                           /// Cursor position
	int mouseSensitivity;                      /// Overall mouse move sensitivity
	int doubleClickDelay;                      /// Delay for double clicks

	int mClickCount;                           /// Internal: used for double click detection
	int lastMousePressTime;                    /// Internal: timestamp of mouse last press
	int lastMousePressButton;                  /// Internal: last pressed button
	int lastMouseDragButton;                   /// Internal: last button used for dragging
	vec2i lastMousePos;                        /// Internal: last mouse position

	queue_t *mouseEventQueue;                  /// All mouse generated input first goes here
	guiMouseInput *mouseInput, *oldMouseInput; /// Mouse input data for current and last tic
} guiMouse;

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
void mouse_init(guiMouse *input);

/**
 * @brief Draws the current cursor
 *
 * Put full description here
 *
 * @param mouse Mouse subsystem
 * @param graphics Graphics subsystem
 */
void mouse_drawCursor(guiMouse *mouse, guiGraphics *graphics);

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
void mouse_registerCursor(guiMouse *mouse, __str image, int width, int height, int hotspotX, int hotspotY);

/**
 * @brief Put brief description here
 *
 * Put full description here
 *
 * @param mouse Put argument desc here
 * @return Put return information here
 */
void mouse_grabMouseInput(guiMouse *mouse);
void mouse_releaseMouseInput(guiMouse *mouse);
void mouse_getInput(guiMouse *input, guiGraphics *graphics);
#define mouse_getSensitivity(_mouse) (((guiMouse*)_mouse)->mouseSensitivity)
#define mouse_setSensitivity(_mouse, _sens) { ((guiMouse*)_mouse)->mouseSensitivity = _sens; }
#define mouse_getLastPressButton(_mouse) (((guiMouse*)_mouse)->lastMousePressButton)
#define mouse_setLastPressButton(_mouse, _button) { ((guiMouse*)_mouse)->lastMousePressButton = _button; }
#define mouse_getLastDragButton(_mouse) (((guiMouse*)_mouse)->lastMouseDragButton)
#define mouse_setLastDragButton(_mouse, _button) { ((guiMouse*)_mouse)->lastMouseDragButton = _button; }

#endif // MOUSE_H_INCLUDED
