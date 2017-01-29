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
	struct guiRectangle *(*w_getChildrenArea)(const struct guiContainer *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiContainer *widget, vec2i pos);
	void (*w_draw)(const struct guiContainer *container, guiGraphics *graphics);
	void (*w_tick)(struct guiContainer *widget);
	bool (*w_isWidgetExisting)(struct guiContainer *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiContainer *widget, void *focus);
	struct vec2i* (*w_getMinimalSize)(const struct guiWidget *widget);

	void (*c_showWidgetPart)(struct guiContainer *container, guiWidget *widget, guiRectangle area);
} guiContainer_vf_t;

typedef struct guiContainer {
	guiWidget widget;

	// All contained widgets
	list_t *children;
	guiLifecycleListener *listener;

} guiContainer;

// Virtual inherited from guiWidget
const char *container_typename(guiContainer *widget);
void container_draw(const guiContainer *widget, guiGraphics *graphics);
guiRectangle *container_getChildrenArea(const guiContainer *widget);
guiWidget *container_getWidgetAt(const guiContainer *widget, vec2i pos);
void container_tick(guiContainer *widget);
bool container_isWidgetExisting(guiContainer *widget, const guiWidget *exist);
void container_setFocusManager(guiContainer *widget, void *focus);

void container_showWidgetPart(guiContainer *container, guiWidget *widget, guiRectangle area);

// Constructor
guiContainer *container_new(guiGUI *gui);
void container_clear(guiContainer *container);
void container_destructor(guiContainer *container);
void container_init(guiContainer *container);
void container_death(void *widget, guiLifecycleEvent *event);
void container_add(guiContainer *container, guiWidget *widget);
void container_addAt(guiContainer *container, guiWidget *widget, vec2i pos);
#define container_addAtXY(_container, _widget, _x, _y) container_addAt((guiContainer*)_container, (guiWidget*)_widget, (vec2i){_x, _y})
void container_remove(guiContainer *container, guiWidget *widget);
void container_moveToTop(guiContainer *container, guiWidget *widget);
void container_moveToBottom(guiContainer *container, guiWidget *widget);


#endif // WIDGETS_CONTAINER_H_INCLUDED
