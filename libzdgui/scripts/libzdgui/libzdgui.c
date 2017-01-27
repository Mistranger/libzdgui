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
	guiMouse_t *mouse = new(guiMouse_t);
	mouse_init(mouse);
	guiGUI_t *gui = new(guiGUI_t);
	gui_init(gui);
	gui_setGraphics(gui, graphics);
	gui_setInput(gui, input);
	gui_setMouse(gui, mouse);
	gui_setTop(gui, NULL);
	guiDebugPrint("gui init");
	
	mouse_registerCursor(gui->mouse, s"DEFCURS", 32, 32, 0, 0);
	string_t *str = string_new_char("Hello world");
	guiImage_t panel = {288, 256, s"panel_2"};
	guiWindow_t *window = window_new(gui, str, &panel);
	guiRectangle_t rect = {100, 100, 288, 256};
	widget_setDimension((guiWidget_t*)window, rect);
	gui_setTop(gui, (guiWidget_t*)window);
	
	string_assign_char(str, "shithappens");
	guiButton_t *button = button_new(gui, str);
	vec2i_t pos = {0, 0};
	
	string_assign_char(str, "shithappens222");
	guiButton_t *button2 = button_new(gui, str);
	pos.x = 0;
	pos.y = 10;
	
	string_assign_char(str, "wowshithappens333");
	guiButton_t *button3 = button_new(gui, str);
	pos.x = 0;
	pos.y = 60;
	
	guiImageWidget_t *image = wimage_new(gui, s"TITLEPIC", 320, 200);
	//image->imageFlags = IF_SCALETOSIZE;
	widget_setSize(image, 320, 200);
	pos.x = 20;
	pos.y = 20;
	//container_addAt((guiContainer_t*)window, (guiWidget_t*)image, pos);
	
	guiFont_t *f = font_new(s"CONFONT", 8, 8);
	string_assign_char(str, "ka Taisen Net Gimmick: Capcom & Psikyo All Stars (Japan))");
	//guiLabel_t *label = label_new(gui, str, f);

	//widget_setSize(label, 200, 100);
	pos.x = 50;
	pos.y = 20;
	//container_addAt((guiContainer_t*)window, (guiWidget_t*)label, pos);
	
	guiImage_t *slider_vert = &(guiImage_t){13, 170, s"SLIDVERT"};
	guiImage_t *scroll_knob = &(guiImage_t){20, 20, s"SLIDKNOB"};
	guiImage_t *scroll_down_pressed = &(guiImage_t){20, 20, s"SCDOWNPR"};
	guiImage_t *scroll_down = &(guiImage_t){20, 20, s"SCDOWN"};
	guiImage_t *scroll_up_pressed = &(guiImage_t){20, 20, s"SCUPPR"};
	guiImage_t *scroll_up = &(guiImage_t){20, 20, s"SCUP"};
	guiImage_t *scroll_left = &(guiImage_t){20, 20, s"SCLEFT"};
	guiImage_t *scroll_left_pressed = &(guiImage_t){20, 20, s"SCLEFTPR"};
	guiImage_t *scroll_right_pressed = &(guiImage_t){20, 20, s"SCRITEPR"};
	guiImage_t *scroll_right = &(guiImage_t){20, 20, s"SCRITE"};
	guiImage_t *slider_horiz = &(guiImage_t){170, 13, s"SLIDHORZ"};
	
	guiScrollArea_t *scroll = scroll_new(gui, (guiWidget_t*)image);
	scroll_setDownButtonImage(scroll, scroll_down);
	scroll_setDownPressedButtonImage(scroll, scroll_down_pressed);
	scroll_setUpButtonImage(scroll, scroll_up);
	scroll_setUpPressedButtonImage(scroll, scroll_up_pressed);
	scroll_setLeftButtonImage(scroll, scroll_left);
	scroll_setLeftPressedButtonImage(scroll, scroll_left_pressed);
	scroll_setRightButtonImage(scroll, scroll_right);
	scroll_setRightPressedButtonImage(scroll, scroll_right_pressed);
	scroll_setMarkerImage(scroll, scroll_knob);
	scroll_setHBarImage(scroll, slider_horiz);
	scroll_setVBarImage(scroll, slider_vert);
	widget_setSize(scroll, 150, 100);
	pos.x = 20;
	pos.y = 20;
	//image->widget.flags &= ~WF_VISIBLE;
	
	container_addAtXY((guiContainer_t*)window, (guiWidget_t*)button, 0, 20);
	container_addAtXY((guiContainer_t*)window, (guiWidget_t*)button2, 0, 40);
	container_addAtXY((guiContainer_t*)window, (guiWidget_t*)button3, 0, 120);
	container_addAtXY((guiContainer_t*)window, (guiWidget_t*)scroll, 20, 20);
	
	mouse_grabMouseInput(gui->mouse);
	while (1) {
		/*int *m = new(int);
		guiDebugPrint("High mem pointer: %p" _C_ m);
		free(m);*/
		gui_tick(gui);
		gui_draw(gui);
		ACS_Delay(1);
	}
	return;
}

#endif

