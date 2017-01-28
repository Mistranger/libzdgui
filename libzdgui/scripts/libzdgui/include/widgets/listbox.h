#ifndef WIDGETS_LISTBOX_H_INCLUDED
#define WIDGETS_LISTBOX_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "event/event_mouse.h"
#include "event/event_widget.h"
#include "util/string.h"
#include "util/vector.h"
#include "widgets/container.h"
#include "widgets/scrollarea.h"
#include "dimension.h"
#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

struct guiListBox;

extern const char *ListBoxType;

typedef struct guiListBox_vf {
	const char *(*w_typename)(struct guiListBox *widget);
	void (*w_destructor)(struct guiListBox *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *listbox);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void(*w_draw)(const struct guiListBox *listbox, guiGraphics *graphics);
	void(*w_tick)(struct guiWidget *widget);
	bool(*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiWidget *widget, void *focus);
} guiListBox_vf_t;

typedef struct guiListBox {
	guiWidget widget;
	list_t *items;
	int selected;
	__str selectedFontColor;

	guiImage *itemImage;
} guiListBox;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

guiListBox *listbox_new(guiGUI *gui, guiScrollArea *scrollarea);
void listbox_init(guiListBox *listbox, guiScrollArea *scrollarea);
void listbox_destructor(guiListBox *listbox);

// Virtual inherited from guiWidget

const char *listbox_typename(guiListBox *widget);
void listbox_draw(const guiListBox *listbox, guiGraphics *graphics);

void listbox_adjustSize(guiListBox *button);

#define listbox_getSelected(_widget) (((guiListBox*)_widget)->selected)
void listbox_setSelected(guiListBox *listbox, int selected);
#define listbox_getSize(_widget) (list_size(((guiListBox*)_widget)->items))
void listbox_addItem(guiListBox *listbox, const string_t *item);
void listbox_addItemAt(guiListBox *listbox, const string_t *item, size_t index);
void listbox_removeItemAt(guiListBox *listbox, size_t index);

#define listbox_getSelectedColor(_widget) (((guiListBox*)_widget)->selectedFontColor)
#define listbox_setSelectedColor(_widget, _font) { ((guiListBox*)_widget)->selectedFontColor = _font; }

#define scroll_getItemImage(_widget) (((guiListBox*)_widget)->itemImage)
#define scroll_setItemImage(_widget, _image) { ((guiListBox*)_widget)->itemImage = _image; }

void listbox_mousePressed(void *widget, guiMouseEvent *mouseEvent);


#endif // WIDGETS_LISTBOX_H_INCLUDED
