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

struct guiListBox_s;

extern const char *ListBoxType;

typedef struct guiListBox_vf {
	const char *(*w_typename)(struct guiListBox_s *widget);
	void (*w_destructor)(struct guiListBox_s *widget);
	struct guiRectangle_s *(*w_getChildrenArea)(const struct guiWidget_s *listbox);
	struct guiWidget_s *(*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void(*w_draw)(const struct guiListBox_s *listbox, guiGraphics_t *graphics);
	void(*w_tick)(struct guiWidget_s *widget);
	bool(*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
	void(*w_setFocusHandler)(struct guiWidget_s *widget, void *focus);
} guiListBox_vf_t;

typedef struct guiListBox_s {
	guiWidget_t widget;
	list_t *items;
	int selected;

	guiImage_t *itemImage;
} guiListBox_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

guiListBox_t *listbox_new(guiGUI_t *gui, guiScrollArea_t *scrollarea);
void listbox_init(guiListBox_t *listbox, guiScrollArea_t *scrollarea);
void listbox_destructor(guiListBox_t *listbox);

// Virtual inherited from guiWidget_t

const char *listbox_typename(guiListBox_t *widget);
void listbox_draw(const guiListBox_t *listbox, guiGraphics_t *graphics);

void listbox_adjustSize(guiListBox_t *button);

#define listbox_getSelected(_widget) (((guiListBox_t*)_widget)->selected)
void listbox_setSelected(guiListBox_t *listbox, int selected);
#define listbox_getSize(_widget) (list_size(((guiListBox_t*)_widget)->items))
void listbox_addItem(guiListBox_t *listbox, const string_t *item);
void listbox_addItemAt(guiListBox_t *listbox, const string_t *item, size_t index);
void listbox_removeItemAt(guiListBox_t *listbox, size_t index);

#define scroll_getItemImage(_widget) (((guiListBox_t*)_widget)->itemImage)
#define scroll_setItemImage(_widget, _image) { ((guiListBox_t*)_widget)->itemImage = _image; }

void listbox_mousePressed(void *widget, mouseEvent_t *mouseEvent);


#endif // WIDGETS_LISTBOX_H_INCLUDED
