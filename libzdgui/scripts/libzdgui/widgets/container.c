#include "libzdgui.h"

#include "widgets/container.h"

#include "dimension.h"
#include "util/list.h"

void container_init(guiContainer_t* container)
{
	container->widget->guiWidget_draw = container_draw;
	container->widget->guiWidget_getChildrenArea = container_getChildrenArea;
	container->widget->guiWidget_getWidgetAt = container_getWidgetAt;
	container->widget->guiWidget_tick = container_tick;
	container->children = list_init();
}

void container_add(guiContainer_t* container, guiWidget_t* widget)
{
	list_push_back(container->children, (void*)widget);
	widget_setParent(widget, container->widget);
}

void container_remove(guiContainer_t* container, guiWidget_t* widget)
{
	for (listNode_t *node = container->children->head; node != container->children->tail; node = node->next) {
		if ((guiWidget_t*)node->data == widget) {
			// FIXME list_er
		}
	}
}

void container_draw(const guiWidget_t *widget, guiGraphics_t *graphics)
{
	const guiContainer_t *container = (const guiContainer_t *)widget;
	graph_pushClipArea(graphics, widget_getDimensions(widget));
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		if (it->flags & WF_VISIBLE) {
			graph_pushClipArea(graphics, *widget->guiWidget_getChildrenArea(it));
			it->guiWidget_draw(it, graphics);
			graph_popClipArea(graphics);
		}
	}
	graph_popClipArea(graphics);
}

guiRectangle_t* container_getChildrenArea(const guiWidget_t* widget)
{
	guiRectangle_t children = {0, 0, widget->dim.width, widget->dim.height};
	return &children;
}

guiWidget_t* container_getWidgetAt(const guiWidget_t *widget, int x, int y)
{
	const guiContainer_t *container = (const guiContainer_t *)widget;
	guiRectangle_t *r = widget->guiWidget_getChildrenArea(widget);

	if (!rect_isPointInRect(r, x, y)) {
		return NULL;
	}

	x -= r->x;
	y -= r->y;

	for (listNode_t *node = container->children->tail; node; node = node->prev) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		guiRectangle_t rect = widget_getDimensions(it);
		if ((it->flags & WF_VISIBLE) && rect_isPointInRect(&rect, x, y)) {
			return it;
		}
	}

	return NULL;
}

void container_tick(guiWidget_t *widget)
{
	guiContainer_t *container = (guiContainer_t *)widget;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		it->guiWidget_tick(it);
	}
}
