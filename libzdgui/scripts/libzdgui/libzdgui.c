#include "libzdgui.h"

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
	guiDebugPrint("graph init");
	guiInput_t *input = (guiInput_t*)malloc(sizeof(guiInput_t));
	input_init(input);
	guiDebugPrint("input init");
	
	guiGUI_t *gui = (guiGUI_t*)malloc(sizeof(guiGUI_t));
	gui_init(gui);
	gui_setGraphics(gui, graphics);
	gui_setInput(gui, input);
	gui_setTop(gui, NULL);
	guiDebugPrint("gui init");
	guiContainer_t *container = container_new();
	guiRectangle_t rect = {100, 100, 200, 200};
	widget_setDimension((guiWidget_t*)container, rect);
	gui_setTop(gui, (guiWidget_t*)container);
	guiButton_t *button = button_new("shithappens");
	container_addAt(container, (guiWidget_t*)button, 0, 0);
	guiButton_t *button2 = button_new("shithappens222");
	container_addAt(container, (guiWidget_t*)button2, 0, 10);
	guiButton_t *button3 = button_new("wowshithappens333");
	container_addAt(container, (guiWidget_t*)button3, 0, 60);
	guiImage_t *image = image_new(s"TITLEPIC", 320, 200);
	image->imageFlags = IF_SCALETOSIZE;
	widget_setSize((guiWidget_t*)image, 240, 150);
	container_addAt(container, (guiWidget_t*)image, 20, 20);
	while (1) {
		/*int xMove = ACS_GetPlayerInput(-1, INPUT_YAW);
		int yMove = ACS_GetPlayerInput(-1, INPUT_PITCH);
		
		graph_drawText(graphics, 0, 0, "%d", xMove);
		graph_drawText(graphics, 0, 50, "%d", yMove);
		 * */
		
		gui_tick(gui);
		gui_draw(gui);
		ACS_Delay(1);
	}
	return;
}


