#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED

#include "event/event_lifecycle.h"
#include "gui.h"
#include "util/list.h"
#include "util/util.h"
#include "widget.h"

struct guiContainer_s;

typedef struct guiContainer_vf {
	void (*w_destructor)(struct guiContainer_s *widget);
	struct guiRectangle_s* (*w_getChildrenArea)(const struct guiContainer_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiContainer_s *widget, vec2i_t pos);
	void (*w_draw)(const struct guiContainer_s *container, guiGraphics_t *graphics);
	void (*w_tick)(struct guiContainer_s *widget);
	bool (*w_isWidgetExisting)(struct guiContainer_s *widget, const struct guiWidget_s *exist);
	
	void (*c_showWidgetPart)(struct guiContainer_s *container, guiRectangle_t area);
} guiContainer_vf_t;

typedef struct guiContainer_s
{
    guiWidget_t widget;
	
	// All contained widgets
	list_t *children;
	lifecycleListener_t *listener;
	
} guiContainer_t;

// Virtual inherited from guiWidget_t
void container_draw(const guiContainer_t *widget, guiGraphics_t *graphics);
guiRectangle_t* container_getChildrenArea(const guiContainer_t *widget);
guiWidget_t* container_getWidgetAt(const guiContainer_t *widget, vec2i_t pos);
void container_tick(guiContainer_t *widget);
bool container_isWidgetExisting(guiContainer_t *widget, const guiWidget_t *exist);

void container_showWidgetPart(guiContainer_t *container, guiRectangle_t area);

// Constructor
guiContainer_t* container_new(guiGUI_t *gui);
void container_destructor(guiContainer_t *container);
void container_init(guiContainer_t *container);
void container_death(void *widget, lifecycleEvent_t *event);
void container_add(guiContainer_t *container, guiWidget_t *widget);
void container_addAt(guiContainer_t *container, guiWidget_t *widget, vec2i_t pos);
void container_remove(guiContainer_t *container, guiWidget_t *widget);
void container_moveToTop(guiContainer_t *container, guiWidget_t *widget);
void container_moveToBottom(guiContainer_t *container, guiWidget_t *widget);


#endif // CONTAINER_H_INCLUDED
