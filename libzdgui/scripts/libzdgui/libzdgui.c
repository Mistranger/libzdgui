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
#if 1
[[call("ScriptS"), script("Enter")]]
void initLibZDGui(void)
{
	guiGraphics_t *graphics = new(guiGraphics_t);
	graph_init(graphics, 640, 480);
	guiDebugPrint("graph init");
	guiInput_t *input = new(guiInput_t);
	input_init(input);
	guiDebugPrint("input init");
	
	guiGUI_t *gui = new(guiGUI_t);
	gui_init(gui);
	gui_setGraphics(gui, graphics);
	gui_setInput(gui, input);
	gui_setTop(gui, NULL);
	guiDebugPrint("gui init");
	string_t *str = string_new_char("Hello world");
	guiImage_t panel = {288, 256, s"panel_2"};
	guiWindow_t *window = window_new(str, &panel);
	guiRectangle_t rect = {100, 100, 200, 200};
	widget_setDimension((guiWidget_t*)window, rect);
	gui_setTop(gui, (guiWidget_t*)window);
	guiButton_t *button = button_new("shithappens");
	vec2i_t pos = {0, 0};
	container_addAt((guiContainer_t*)window, (guiWidget_t*)button, pos);
	guiButton_t *button2 = button_new("shithappens222");
	pos.x = 0;
	pos.y = 10;
	container_addAt((guiContainer_t*)window, (guiWidget_t*)button2, pos);
	guiButton_t *button3 = button_new("wowshithappens333");
	pos.x = 0;
	pos.y = 60;
	container_addAt((guiContainer_t*)window, (guiWidget_t*)button3, pos);
	guiImageWidget_t *image = wimage_new(s"TITLEPIC", 320, 200);
	image->imageFlags = IF_SCALETOSIZE;
	widget_setSize(image, 80, 80);
	pos.x = 20;
	pos.y = 20;
	container_addAt((guiContainer_t*)window, (guiWidget_t*)image, pos);
	
	guiFont_t *f = font_new(s"CONFONT", 8, 8);
	string_assign_char(str, "ka Taisen Net Gimmick: Capcom & Psikyo All Stars (Japan))");
	guiLabel_t *label = label_new(str, f);

	widget_setSize(label, 200, 100);
	pos.x = 50;
	pos.y = 20;
	image->widget.flags &= ~WF_VISIBLE;
	container_addAt((guiContainer_t*)window, (guiWidget_t*)label, pos);
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

#endif

