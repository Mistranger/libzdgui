#include "libzdgui.h"

#include "graphics.h"
#include "gui.h"
#include "input.h"

int ACS_HudMessage(int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, fixed oarg3, str format, ...)
{
	int ret = 0;
	va_list arg;
	
	ACS_BeginHudMessage();
		
	va_start(arg, format);
	ret = __vnprintf_str(format, arg);
	va_end(arg);
	
	ACS_MoreHudMessage();
	ACS_OptHudMessage(flags, id, color, x, y, holdTime);
	ACS_EndHudMessageXXX(oarg1, oarg2, oarg3);
	
	return ret;
}

int ACS_Print (const char *format, ...)
{
	int ret;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_EndPrint();
	return ret;
}

/*
__str ACS_StrParam (const char *format, ...)
{
	va_list arg;
	
	ACS_BeginStrParam();
	
	va_start(arg, format);
	__vnprintf(format, arg);
	va_end(arg);
	
	return ACS_EndStrParam();
}*/

[[call("ScriptS"), script("Enter")]]
void initLibZDGui(void)
{
	guiGraphics_t *graphics = (guiGraphics_t*)malloc(sizeof(guiGraphics_t));
	graph_init(graphics, 640, 480);
	
	guiInput_t *input = (guiInput_t*)malloc(sizeof(guiInput_t));
	input_init(input);
	
	
	guiGUI_t *gui = (guiGUI_t*)malloc(sizeof(guiGUI_t));
	gui_init(gui);
	gui_setGraphics(gui, graphics);
	gui_setInput(gui, input);
	gui_setTop(gui, NULL);
	while (1) {
		int xMove = ACS_GetPlayerInput(-1, INPUT_YAW);
		int yMove = ACS_GetPlayerInput(-1, INPUT_PITCH);
		
		graph_drawText(graphics, 0, 0, "%d", xMove);
		graph_drawText(graphics, 0, 50, "%d", yMove);
		ACS_Delay(1);
	}
	return;
}
