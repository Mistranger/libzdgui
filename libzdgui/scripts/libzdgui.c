#include "libzdgui.h"

guiScrollArea *scroll2;
guiListBox *lbox;
guiLabel *label1;

void musiclistbox_valueChanged(void *widget, guiWidgetEvent *widgetEvent)
{
	guiListBox *listbox = (guiListBox*)widget;
	string_t *selected = (string_t*)(list_get(listbox->items, listbox_getSelected(listbox))->data);
	__str sstr = CharToStr(string_cstr(selected));
	ACS_LocalSetMusic(sstr);
	string_t *str = string_new_char("");
	string_assign_char(str, "Playing: ");
	string_append_string(str, selected);
	label_setCaption(label1, str);
	string_delete(str);
}

void musicslider_valueChanged(void *widget, guiWidgetEvent *widgetEvent)
{
	guiSlider *slider = (guiSlider *)widget;
	ACS_SetMusicVolume((fixed)slider_getValue(slider) / 100);
}

void createMusicPlayer(guiGUI *gui)
{
	string_t *str = string_new_char("Doom music player");
	guiWindow *window = window_new(gui, str);
	guiRectangle rect = {0, 0, 200, 250};
	widget_setDimension((guiWidget *)window, rect);
	
	scroll2 = scroll_new(gui, NULL);
	lbox = listbox_new(gui, scroll2);
	widget_setFont(lbox, (guiFont*)&FONT_CONFONT);
	widget_setFontColor(lbox, s"White");
	

	const char* music[] = {"D_RUNNIN", "D_STALKS", "D_COUNTD", "D_BETWEE", "D_DOOM", "D_THE_DA", "D_SHAWN", "D_DDTBLU",
		"D_IN_CIT","D_DEAD","D_ROMERO","D_MESSAG","D_AMPIE", "D_ADRIAN", "D_TENSE", "D_OPENIN", "D_EVIL","D_ULTIMA",
		"D_DM2TTL","D_DM2INT","D_READ_M"};
	for (int i = 0; i < sizeof(music)/sizeof(const char*); ++i) {
		string_assign_char(str, music[i]);
		listbox_addItem(lbox, str);
	}
	
	listbox_adjustSize(lbox);
	widget_setWidth(lbox, 150);
	widget_setSize(scroll2, 150, 100);
	scroll_setScrollPolicy(scroll2, SAP_NEVER, SAP_AUTO);
	guiWidgetListener *changed = widgetListener_new(lbox, WE_VALUE_CHANGED, musiclistbox_valueChanged);
	widget_addListener(lbox, changed);
	
	guiSlider *slider = slider_new(gui, 200);
	widget_setSize(slider, 150, 13);
	guiWidgetListener *sliderchanged = widgetListener_new(slider, WE_VALUE_CHANGED, musicslider_valueChanged);
	widget_addListener(slider, sliderchanged);
	string_assign_char(str, "Playing: ");
	label1 = label_new(gui, str, (guiFont*)&FONT_CONFONT);
	widget_setSize(label1, 150, 13);
	string_assign_char(str, "Volume:");
	guiLabel *label2 = label_new(gui, str, (guiFont*)&FONT_CONFONT);
	widget_setSize(label2, 150, 13);
	container_addAtXY(window, scroll2, 20, 20);
	container_addAtXY(window, label1, 20, 150);
	container_addAtXY(window, label2, 20, 185);
	container_addAtXY(window, slider, 20, 200);
	container_addAtXY(gui_getTop(gui), window, 100, 20);
}

void createTestWindow(guiGUI *gui)
{
	string_t *str = string_new_char("Test Window");

	guiWindow *window = window_new(gui, str);

	guiRectangle rect = {100, 100, 288, 256};
	widget_setDimension((guiWidget *)window, rect);
	

	string_assign_char(str, "Button");
	guiButton *button = button_new(gui, str);
	widget_setFontColor(button, s"White");
	
	guiImageWidget *image = wimage_new(gui, s"TITLEPIC", 320, 200);
	widget_setSize(image, 320, 200);

	guiFont *f = font_new(s"CONFONT", 8, 8);
	string_assign_char(str, "This function takes a string and an output buffer and a desired width. It then copies\
   the string to the buffer, inserting a new line character when a certain line\
   length is reached.  If the end of the line is in the middle of a word, it will\
   backtrack along the string until white space is found.");
	guiLabel *label = label_new(gui, str, f);
	label->isMultiline = true;
	
	widget_setSize(label, 150, 100);

	guiScrollArea *scroll = scroll_new(gui, (guiWidget *)image);
	widget_setSize(scroll, 200, 120);

	string_assign_char(str, "Awesome");
	guiCheckBox *cbox = checkbox_new(gui, str);
	checkbox_adjustSize(cbox);
	container_addAtXY(window, cbox, 20, 20);
	string_assign_char(str, "To be");
	guiRadioButton *radio1 = radio_new(gui, str);
	radio_adjustSize(radio1);
	container_addAtXY(window, radio1, 20, 40);
	string_assign_char(str, "Not to be");
	guiRadioButton *radio2 = radio_new(gui, str);
	radio_adjustSize(radio2);
	
	container_addAtXY(window, radio2, 20, 60);
	container_addAtXY(window, button, 20, 100);
	container_addAtXY(window, scroll, 10, 120);
	container_addAtXY(window, label, 120, 20);
	container_addAtXY(gui_getTop(gui), window, 150, 70);
}

#if 1
[[call("ScriptS"), script("Enter")]]
void initLibZDGui(void)
{
	ACS_PrintLogStr(s"\CdlibZDGUI v%s by cybermind aka Mistranger", libzdgui_getVersion());
	guiGUI *gui = new (guiGUI);
	gui_init(gui, 640, 480);
	gui_setTop(gui, NULL);

	mouse_registerCursor(gui_getMouse(gui), s"DEFCURS", 32, 32, 0, 0);
	guiContainer *cont = container_new(gui);
	widget_setSize(cont, 640, 480);
	gui_setTop(gui, (guiWidget *)cont);
	
	createMusicPlayer(gui);
	createTestWindow(gui);
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

