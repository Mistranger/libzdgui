#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED

#include "util/list.h"
#include "util/util.h"
#include "widget.h"

struct guiContainer_s;

typedef struct guiContainer_vf {
	
	struct guiRectangle_s (*w_getChildrenArea)(const struct guiContainer_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiContainer_s *widget, int x, int y);
	void (*w_draw)(const struct guiContainer_s *container, guiGraphics_t *graphics);
	void (*w_tick)(struct guiContainer_s *widget);
	bool (*w_isWidgetExisting)(struct guiContainer_s *widget, const struct guiWidget_s *exist);
} guiContainer_vf_t;

typedef struct guiContainer_s
{
    guiWidget_t widget;
	
	// All contained widgets
	list_t *children;
	
} guiContainer_t;

// Virtual inherited from guiWidget_t
void container_draw(const guiContainer_t *widget, guiGraphics_t *graphics);
guiRectangle_t container_getChildrenArea(const guiContainer_t *widget);
guiWidget_t* container_getWidgetAt(const guiContainer_t *widget, int x, int y);
void container_tick(guiContainer_t *widget);
bool container_isWidgetExisting(guiContainer_t *widget, const guiWidget_t *exist);

// Constructor
guiContainer_t* container_new();
void container_init(guiContainer_t *container);
void container_add(guiContainer_t *container, guiWidget_t *widget);
void container_addAt(guiContainer_t *container, guiWidget_t *widget, int x, int y);
void container_remove(guiContainer_t *container, guiWidget_t *widget);
void container_moveToTop(guiContainer_t *container, guiWidget_t *widget);
void container_moveToBottom(guiContainer_t *container, guiWidget_t *widget);


#endif // CONTAINER_H_INCLUDED
