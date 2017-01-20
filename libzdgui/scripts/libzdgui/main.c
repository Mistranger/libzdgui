#include "libzdgui.h.bcs"

#define MAXGUI 1024
#define MAXEXTRA 32

/*
--------------------------------------------------------------------------------------------------------
	Configuration parameters
--------------------------------------------------------------------------------------------------------
*/



/*
--------------------------------------------------------------------------------------------------------
	Debug macros
--------------------------------------------------------------------------------------------------------
*/

#define DEBUG



/*
--------------------------------------------------------------------------------------------------------
	Speed-up macros for speed-critical parts
--------------------------------------------------------------------------------------------------------
*/

/**
**	Checks, if the mouse is within picked element.
**
**	@param        GID: Picked widget
**	@param        x: x coord of mouse.
**	@param        y: y coord of mouse.
**
**	@return       1 if mouse is within element, 0 otherwise.
**
**/

#define GUI_IsMouseInsideElement(GID, x, y) \
	((#x >= GUIPool[#GID].ax && #x < GUIPool[#GID].ax + GUIPool[#GID].width && #y >= GUIPool[#GID].ay && #y < GUIPool[#GID].ay + GUIPool[#GID].height) \
	? 1 \
	: 0)
	
#define GUI_IsElementInsideElement(GID, inside) \
	((GUIPool[#GID].ax >= GUIPool[#inside].ax && GUIPool[#GID].ax < GUIPool[#inside].ax + GUIPool[#inside].width \
	&& GUIPool[#GID].ay >= GUIPool[#inside].ay && GUIPool[#GID].ay < GUIPool[#inside].ay + GUIPool[#inside].height) \
	? 1 \
	: 0)

/*
--------------------------------------------------------------------------------------------------------
	Basic widget structure
--------------------------------------------------------------------------------------------------------
*/

global GUIInfo_T 1:GUIPool[];
global int 2:GUILast;
global int 3:FreeGUICount;

/*
--------------------------------------------------------------------------------------------------------
	GUI flags
--------------------------------------------------------------------------------------------------------
*/

enum GUIFlags {
	GF_ENABLED = 0x00000001
};

/*
	GUI internal flags
*/

enum GUIInternalFlags {
	GIF_INIT = 0x00000001
};

/*
--------------------------------------------------------------------------------------------------------
	Mouse configuration
--------------------------------------------------------------------------------------------------------
*/

enum MouseState {
   MS_NORMAL = 0,
   MS_PRESS,
   MS_REPEAT,
   MS_RELEASE
};

struct MouseInfo_T {
	int CursorX;
	int CursorY;
	int OldCursorX;
	int OldCursorY;
	MouseState LeftMouseState;
	MouseState RightMouseState;
};

MouseInfo_T Mouseinfo;

/*
--------------------------------------------------------------------------------------------------------
	Drag-and-drop
--------------------------------------------------------------------------------------------------------
*/




/*
--------------------------------------------------------------------------------------------------------
	Generic widget managenent
--------------------------------------------------------------------------------------------------------
*/




function int GUI_AllocNewElement(void)
{
	int freeGUI = 0;
	if (FreeGUICount) {
		do {
			++freeGUI;
		} while (GUIPool[freeGUI].id != 0);
		--FreeGUICount;
	} else {
		freeGUI = ++GUILast;
	}
	
	return freeGUI;
}

function int GUI_CreateNewElement(
	int width, int height, int flags,
	int dragx, int dragy, int dragwidth, int dragheight)
{
	int newElement = GUI_AllocNewElement();
	if (!newElement) {
		//GUI_ErrorMessage("GUI_CreateNewElement", "cannot allocate new element");
		return -1;
	}
	
	GUIPool[newElement].id = newElement;
	GUIPool[newElement].width = width;
	GUIPool[newElement].height = height;
	GUIPool[newElement].flags = flags;
	// Mark this element as non-ready
	GUIPool[newElement].iflags = GUIPool[newElement].iflags | GIF_INIT;	
	
	
	return newElement;
}
