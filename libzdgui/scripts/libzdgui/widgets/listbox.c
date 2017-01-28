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

const guiImage listDefImageItem = {200, 19, s"LISTITEM"};

const char *ListBoxType = "ListBox";

const char *listbox_typename(guiListBox *widget)
{
	return ListBoxType;
}

guiListBox *listbox_new(guiGUI *gui, guiScrollArea *scrollarea)
{
	guiListBox *listbox = new (guiListBox);
	listbox_init(listbox, scrollarea);
	gui_addWidget(gui, listbox);
	return listbox;
}

void listbox_init(guiListBox *listbox, guiScrollArea *scrollarea)
{
	widget_init((guiWidget *)listbox);
	listbox->widget.v = (guiWidget_vf_t *)&guiListBox_vtable;

	listbox->selected = -1;
	listbox->items = list_new();
	listbox->selectedFontColor = s"Yellow";
	listbox->itemImage = (guiImage*)&listDefImageItem;
	scroll_setContent(scrollarea, (guiWidget *)listbox);
}

void listbox_destructor(guiListBox *listbox)
{
	list_clear(listbox->items);
	widget_destructor((guiWidget *)listbox);
}

void listbox_draw(const guiListBox *listbox, guiGraphics *graphics)
{
	if (!listbox_getSize(listbox)) {
		return;
	}

	guiImage *img = listbox->itemImage;
	int height = (listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
										   image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox)));

	int i = 0;
	int y = 0;
	for (listNode_t *node = listbox->items->head; node; node = node->next) {
		string_t *s = (string_t *)node->data;
		if (img) {
			graph_drawImage(graphics, 0, y, image_getImage(*img));
		}
		if (i == listbox_getSelected(listbox)) {
			graph_drawText(graphics, widget_getFont(listbox), 1, y + (height - font_getCharHeight(*widget_getFont(listbox))),
				listbox_getSelectedColor(listbox), string_cstr(s));
		} else {
			graph_drawText(graphics, widget_getFont(listbox), 1, y + (height - font_getCharHeight(*widget_getFont(listbox))), 
				widget_getFontColor(listbox), string_cstr(s));
		}
		y += height;
	}
}

void listbox_adjustSize(guiListBox *listbox)
{
	int i, l;
	int width = widget_getWidth(listbox);

	for (listNode_t *node = listbox->items->head; node; node = node->next) {
		string_t *s = (string_t *)node->data;
		l = font_getWidthString(widget_getFont(listbox), s, false);
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

void listbox_setSelected(guiListBox *listbox, int selected)
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

		guiWidget *par = widget_getParent(listbox);
		if (!par || !par->isContainer) {
			return;
		}

		guiRectangle scroll;
		guiScrollArea *scrollarea = (guiScrollArea *)par;
		scroll.pos.y = (listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
												 image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox))) * listbox->selected;
		scroll.height = (listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
												  image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox)));
		((guiContainer_vf_t *)(((guiContainer *)par)->widget.v))->c_showWidgetPart((guiContainer *)par, (guiWidget *)listbox, scroll);
	}

}

void listbox_addItem(guiListBox *listbox, const string_t *item)
{
	string_t *add = string_new_string(item);
	list_push_back(listbox->items, add);
	listbox_adjustSize(listbox);
}

void listbox_addItemAt(guiListBox *listbox, const string_t *item, size_t index)
{
	string_t *add = string_new_string(item);
	list_insert(listbox->items, list_get(listbox->items, index), true, add);
	listbox_adjustSize(listbox);
}

void listbox_removeItemAt(guiListBox *listbox, size_t index)
{
	list_erase(listbox->items, list_get(listbox->items, index));
	listbox_adjustSize(listbox);
}

void listbox_mousePressed(void *widget, guiMouseEvent *mouseEvent)
{
	guiListBox *listbox = (guiListBox *)widget;
	if (mouseEvent->button == MB_LEFT) {
		listbox_setSelected(listbox, mouseEvent->pos.y /
							(listbox->itemImage ? MAX(font_getCharHeight(*widget_getFont(listbox)),
													  image_getHeight(*listbox->itemImage)) : font_getCharHeight(*widget_getFont(listbox))));
		guiWidgetEvent *changed = widgetEvent_new(listbox, WE_VALUE_CHANGED);
		widget_handleEvent((guiWidget *)listbox, (guiEvent *)changed, true);
	}
}

