#include "libzdgui.h"

#if 1
[[call("ScriptS"), script("Enter")]]
void initLibZDGui(void)
{
	
	guiGUI *gui = new (guiGUI);
	gui_init(gui, 640, 480);
	gui_setTop(gui, NULL);
	

	mouse_registerCursor(gui_getMouse(gui), s"DEFCURS", 32, 32, 0, 0);
	guiContainer *cont = container_new(gui);
	widget_setSize(cont, 640, 480);
	string_t *str = string_new_char("Hello world");
	guiImage panel = {288, 256, s"panel_2"};
	guiWindow *window = window_new(gui, str, &panel);
	guiRectangle rect = {100, 100, 288, 256};
	widget_setDimension((guiWidget *)window, rect);
	gui_setTop(gui, (guiWidget *)cont);

	string_assign_char(str, "shithappens");
	guiButton *button = button_new(gui, str);
	vec2i pos = {0, 0};

	string_assign_char(str, "shithappens222");
	guiButton *button2 = button_new(gui, str);
	pos.x = 0;
	pos.y = 10;

	string_assign_char(str, "wowshithappens333");
	guiButton *button3 = button_new(gui, str);
	pos.x = 0;
	pos.y = 60;

	guiImageWidget *image = wimage_new(gui, s"TITLEPIC", 320, 200);
	//image->imageFlags = IF_SCALETOSIZE;
	widget_setSize(image, 320, 200);
	pos.x = 20;
	pos.y = 20;
	//container_addAt((guiContainer_t*)window, (guiWidget*)image, pos);

	guiFont *f = font_new(s"CONFONT", 8, 8);
	string_assign_char(str, "ka Taisen Net Gimmick: Capcom & Psikyo All Stars (Japan))");
	guiLabel *label = label_new(gui, str, f);
	
	widget_setSize(label, 100, 100);
	pos.x = 50;
	pos.y = 20;
	

	guiImage *slider_vert = &(guiImage) {13, 170, s"SLIDVERT"};
	guiImage *scroll_knob = &(guiImage) {20, 20, s"SLIDKNOB"};
	guiImage *scroll_down_pressed = &(guiImage) {20, 20, s"SCDOWNPR"};
	guiImage *scroll_down = &(guiImage) {20, 20, s"SCDOWN"};
	guiImage *scroll_up_pressed = &(guiImage) {20, 20, s"SCUPPR"};
	guiImage *scroll_up = &(guiImage) {20, 20, s"SCUP"};
	guiImage *scroll_left = &(guiImage) {20, 20, s"SCLEFT"};
	guiImage *scroll_left_pressed = &(guiImage) {20, 20, s"SCLEFTPR"};
	guiImage *scroll_right_pressed = &(guiImage) {20, 20, s"SCRITEPR"};
	guiImage *scroll_right = &(guiImage) {20, 20, s"SCRITE"};
	guiImage *slider_horiz = &(guiImage) {170, 13, s"SLIDHORZ"};

	guiScrollArea *scroll = scroll_new(gui, (guiWidget *)image);
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

	guiScrollArea *scroll2 = scroll_new(gui, NULL);
	guiListBox *lbox = listbox_new(gui, scroll2);
	widget_setSize(lbox, 150, 100);
	widget_setSize(scroll2, 150, 100);
	string_assign_char(str, "item1");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item2");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	string_assign_char(str, "item3");
	listbox_addItem(lbox, str);
	scroll_setDownButtonImage(scroll2, scroll_down);
	scroll_setDownPressedButtonImage(scroll2, scroll_down_pressed);
	scroll_setUpButtonImage(scroll2, scroll_up);
	scroll_setUpPressedButtonImage(scroll2, scroll_up_pressed);
	scroll_setLeftButtonImage(scroll2, scroll_left);
	scroll_setLeftPressedButtonImage(scroll2, scroll_left_pressed);
	scroll_setRightButtonImage(scroll2, scroll_right);
	scroll_setRightPressedButtonImage(scroll2, scroll_right_pressed);
	scroll_setMarkerImage(scroll2, scroll_knob);
	scroll_setHBarImage(scroll2, slider_horiz);
	scroll_setVBarImage(scroll2, slider_vert);

	string_assign_char(str, "Awesome checkbox");
	guiCheckBox *cbox = checkbox_new(gui, str);
	checkbox_adjustSize(cbox);
	container_addAtXY(window, cbox, 20, 20);
	string_assign_char(str, "Option 1");
	guiRadioButton *radio1 = radio_new(gui, str);
	radio_adjustSize(radio1);
	container_addAtXY(window, radio1, 20, 40);
	string_assign_char(str, "Option 2");
	guiRadioButton *radio2 = radio_new(gui, str);
	radio_adjustSize(radio2);
	container_addAtXY(window, radio2, 20, 60);
	container_addAtXY(window, button, 0, 100);
	container_addAtXY(window, button2, 0, 120);
	container_addAtXY(window, button3, 0, 140);
	container_addAtXY(window, scroll2, 50, 120);
	container_addAtXY(window, label, 100, 20);
	container_addAtXY(cont, window, 100, 20);
	//container_addAtXY((guiContainer_t*)window, (guiWidget*)scroll, 20, 20);

	mouse_grabMouseInput(gui->mouse);
	while (1) {
		
		gui_tick(gui);
		gui_draw(gui);
		/*int *m = new(int);
		guiInfo("Memory leak test: %p" _C_ m);
		free(m);*/
		ACS_Delay(1);
	}
	return;
}

#endif

