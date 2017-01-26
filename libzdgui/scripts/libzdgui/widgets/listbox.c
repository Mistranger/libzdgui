#include "system.h"
#include "widgets/listbox.h"

#include "event/event_widget.h"
#include "util/util.h"

guiListBox_vf_t guiListBox_vtable = {
	listbox_typename,
	listbox_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	listbox_draw,
	widget_tick,
	widget_isWidgetExisting,
	widget_setFocusManager
};

const char *ListBoxType = "ListBox";

const char* listbox_typename(guiListBox_t *widget)
{
	return ListBoxType;
}

guiListBox_t* listbox_new(guiGUI_t *gui, guiScrollArea_t *scrollarea)
{
	guiListBox_t *listbox = new(guiListBox_t);
	listbox_init(listbox, scrollarea);
	gui_addWidget(gui, listbox);
	return listbox;
}

void listbox_init(guiListBox_t* listbox, guiScrollArea_t *scrollarea)
{
	widget_init((guiWidget_t*)listbox);
	listbox->widget.v = (guiWidget_vf_t*)&guiListBox_vtable;

	listbox->selected = -1;
	listbox->items = list_new();
	listbox->scrollarea = scrollarea;
	scroll_setContent(scrollarea, (guiWidget_t*)listbox);
}

void listbox_destructor(guiListBox_t *listbox)
{
	list_clear(listbox->items);
	widget_destructor((guiWidget_t*)listbox);
}

void listbox_draw(const guiListBox_t* listbox, guiGraphics_t* graphics)
{
	if (!listbox_getSize(listbox)) {
		return;
	}

	guiImage_t *img = listbox->itemImage;
	int height = (listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
		image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox)));

	int i = 0;
	int y = 0;
	for (listNode_t *node = listbox->items->head; node; node = node->next) {
		string_t *s = (string_t*)node->data;
		if (img) {
			graph_drawImage(graphics, 0, 0, image_getImage(*img));
		}
		if (i == listbox_getSelected(listbox)) {
			// FIXME
			graph_drawText(graphics, 1, y + (height - font_getCharHeight(*widget_getFont(listbox))), s->s);
		} else {
			graph_drawText(graphics, 1, y + (height - font_getCharHeight(*widget_getFont(listbox))), s->s);
		}
		y += height;
	}
}

void listbox_adjustSize(guiListBox_t *listbox)
{
	int i, l;
	int width = widget_getWidth(listbox);

	for (listNode_t *node = listbox->items->head; node; node = node->next) {
		string_t *s = (string_t*)node->data;
		l = font_getWidthString(widget_getFont(listbox), s);
		if (width < l) {
			width = l;
		}
	}
	if (width != widget_getWidth(listbox)) {
		widget_setWidth(listbox, width);
	}
	widget_setHeight(listbox, (listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
		image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox))) * listbox_getSize(listbox));
}

void listbox_setSelected(guiListBox_t* listbox, int selected)
{
	if (!list_size(listbox->items)) {
		listbox->selected = -1;
	} else {
		if (listbox->selected < 0) {
			listbox->selected = -1;
		} else if (selected >= listbox_getSize(listbox)) {
			listbox->selected = listbox_getSize(listbox) - 1;
		} else {
			listbox->selected = selected;
		}

		guiWidget_t *par = widget_getParent(listbox);
		if (!par || !par->isContainer) {
			return;
		}

		guiRectangle_t scroll;
		guiScrollArea_t *scrollarea = (guiScrollArea_t*)par;
		scroll.pos.y = (listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
			image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox))) * listbox->selected;
		scroll.height = (listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
			image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox)));
		((guiContainer_vf_t*)(((guiContainer_t*)par)->widget.v))->c_showWidgetPart((guiContainer_t*)par, (guiWidget_t*)listbox, scroll);		
	}

}

void listbox_addItem(guiListBox_t* listbox, const string_t *item)
{
	string_t *add = string_new_string(item);
	list_push_back(listbox->items, add);
	listbox_adjustSize(listbox);
}

void listbox_addItemAt(guiListBox_t* listbox, const string_t *item, size_t index)
{
	string_t *add = string_new_string(item);
	list_insert(listbox->items, list_get(listbox->items, index), true, add);
	listbox_adjustSize(listbox);
}

void listbox_removeItemAt(guiListBox_t* listbox, size_t index)
{
	list_erase(listbox->items, list_get(listbox->items, index));
	listbox_adjustSize(listbox);
}

void listbox_mousePressed(void *widget, mouseEvent_t *mouseEvent)
{
	guiListBox_t *listbox = (guiListBox_t *)widget;
	if (mouseEvent->button == MB_LEFT) {
		listbox_setSelected(listbox, mouseEvent->pos.y / 
			(listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
			image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox))));
		widgetEvent_t *changed = widgetEvent_new(listbox, WE_VALUE_CHANGED);
		widget_handleEvent((guiWidget_t*)listbox, (event_t*)changed);
	}
}

