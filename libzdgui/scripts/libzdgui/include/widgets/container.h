#ifndef WIDGETS_CONTAINER_H_INCLUDED
#define WIDGETS_CONTAINER_H_INCLUDED

#include "event/event_lifecycle.h"
#include "gui.h"
#include "util/list.h"
#include "util/util.h"
#include "widget.h"

extern const char *ContainerType;

struct guiContainer;

typedef struct guiContainer_vf {
	const char *(*w_typename)(struct guiContainer *widget);
	void (*w_destructor)(struct guiContainer *widget);
	struct guiRectangle_s *(*w_getChildrenArea)(const struct guiContainer *widget);
	struct guiWidget_s *(*w_getWidgetAt)(const struct guiContainer *widget, vec2i_t pos);
	void (*w_draw)(const struct guiContainer *container, guiGraphics_t *graphics);
	void (*w_tick)(struct guiContainer *widget);
	bool (*w_isWidgetExisting)(struct guiContainer *widget, const struct guiWidget_s *exist);
	void(*w_setFocusHandler)(struct guiContainer *widget, void *focus);

	void (*c_showWidgetPart)(struct guiContainer *container, guiWidget_t *widget, guiRectangle_t area);
} guiContainer_vf_t;

typedef struct guiContainer {
	guiWidget_t widget;

	// All contained widgets
	list_t *children;
	lifecycleListener_t *listener;

} guiContainer;

// Virtual inherited from guiWidget_t
const char *container_typename(guiContainer *widget);
void container_draw(const guiContainer *widget, guiGraphics_t *graphics);
guiRectangle_t *container_getChildrenArea(const guiContainer *widget);
guiWidget_t *container_getWidgetAt(const guiContainer *widget, vec2i_t pos);
void container_tick(guiContainer *widget);
bool container_isWidgetExisting(guiContainer *widget, const guiWidget_t *exist);
void container_setFocusManager(guiContainer *widget, void *focus);

void container_showWidgetPart(guiContainer *container, guiWidget_t *widget, guiRectangle_t area);

// Constructor
guiContainer *container_new(guiGUI_t *gui);
void container_clear(guiContainer *container);
void container_destructor(guiContainer *container);
void container_init(guiContainer *container);
void container_death(void *widget, lifecycleEvent_t *event);
void container_add(guiContainer *container, guiWidget_t *widget);
void container_addAt(guiContainer *container, guiWidget_t *widget, vec2i_t pos);
#define container_addAtXY(_container, _widget, _x, _y) container_addAt(_container, _widget, (vec2i_t){_x, _y})
void container_remove(guiContainer *container, guiWidget_t *widget);
void container_moveToTop(guiContainer *container, guiWidget_t *widget);
void container_moveToBottom(guiContainer *container, guiWidget_t *widget);


#endif // WIDGETS_CONTAINER_H_INCLUDED
