#include "system.h"

#include "widgets/container.h"

#include "dimension.h"
#include "util/list.h"

guiContainer_vf_t guiContainer_vtable = {
	container_getChildrenArea,
	container_getWidgetAt,
	container_draw,
	container_tick,
	container_isWidgetExisting,
	container_showWidgetPart
};

guiContainer_t* container_new()
{
	guiContainer_t *container = (guiContainer_t*)malloc(sizeof(guiContainer_t));

	container_init(container);
	guiInfo("new container created");
	return container;
}

void container_init(guiContainer_t* container)
{
	widget_init(&container->widget);
	container->widget.v = (guiWidget_vf_t*)&guiContainer_vtable;
	container->children = list_new();
}

void container_add(guiContainer_t* container, guiWidget_t* widget)
{
	list_push_back(container->children, (void*)widget);
	widget_setParent(widget, &container->widget);
}

void container_addAt(guiContainer_t* container, guiWidget_t* widget, vec2i_t pos)
{
	widget_setPosition(widget, pos);
	container_add(container, widget);
	guiInfo("container item added");
}

void container_remove(guiContainer_t* container, guiWidget_t* widget)
{
	for (listNode_t *node = container->children->head; node; node = node->next) {
		if ((guiWidget_t*)node->data == widget) {
			list_erase(container->children, node);
			widget_setParent(widget, NULL);
			return;
		}
	}
}

void container_draw(const guiContainer_t *container, guiGraphics_t *graphics)
{
	guiDebugPrint("drawing container");
	
	graph_pushClipArea(graphics, *container->widget.v->w_getChildrenArea((guiWidget_t*)container));
	ACS_SetHudSize(640, 480, 1);
	//graph_drawImage(graphics, 0, 0, s"HUDFONT_libzdgui_BACK");
	ACS_SetHudSize(graph_getScreenWidth(graphics), graph_getScreenHeight(graphics), 1);
	
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		if (it->flags & WF_VISIBLE) {
			graph_pushClipArea(graphics, widget_getDimensions(it));
			it->v->w_draw(it, graphics);
			graph_popClipArea(graphics);
		}
	}
	
	graph_popClipArea(graphics);
}

guiRectangle_t* container_getChildrenArea(const guiContainer_t* container)
{
	return &(guiRectangle_t){0, 0, container->widget.dim.width, container->widget.dim.height};
}

guiWidget_t* container_getWidgetAt(const guiContainer_t *container, vec2i_t pos)
{
	guiRectangle_t r = *container->widget.v->w_getChildrenArea(&container->widget);

	if (!rect_isPointInRect(&r, pos)) {
		return NULL;
	}

	vec_sub(pos, r.pos);

	for (listNode_t *node = container->children->tail; node; node = node->prev) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		guiRectangle_t rect = widget_getDimensions(it);
		if ((it->flags & WF_VISIBLE) && rect_isPointInRect(&rect, pos)) {
			return it;
		}
	}

	return NULL;
}

void container_tick(guiContainer_t *widget)
{
	guiDebugPrint("container wtick");
	guiContainer_t *container = (guiContainer_t *)widget;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		if (it->v->w_tick != widget_tick) {
			it->v->w_tick(it);
		}
		
	}
}

bool container_isWidgetExisting(guiContainer_t* widget, const guiWidget_t* exist)
{
	guiContainer_t *container = (guiContainer_t *)widget;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		if (it->v->w_isWidgetExisting(it, exist)) {
			return true;
		}
	}
	return false;
}

void container_showWidgetPart(guiContainer_t* container, guiRectangle_t area)
{
	guiRectangle_t widgetArea = *container_getChildrenArea(container);
	area.pos.x += widget_getX(container);
	area.pos.y += widget_getY(container);

	if (area.pos.x + area.width > widgetArea.width) {
		widget_setX(container, widget_getX(container) - area.pos.x - area.width + widgetArea.width);
	}

	if (area.pos.y + area.height > widgetArea.height) {
		widget_setY(container, widget_getY(container) - area.pos.y - area.height + widgetArea.height);
	}

	if (area.pos.x < 0) {
		widget_setX(container, widget_getX(container) - area.pos.x);
	}

	if (area.pos.y < 0) {
		widget_setY(container, widget_getY(container) - area.pos.y);
	}
}

void container_moveToTop(guiContainer_t* container, guiWidget_t* widget)
{
	if (list_size(container->children) < 2) { // nothing in container or already on top
		return;
	}
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		if (it == widget) {
			list_swap(container->children, node, container->children->tail);
		}
	}
}

void container_moveToBottom(guiContainer_t* container, guiWidget_t* widget)
{
	if (list_size(container->children) < 2) { // nothing in container or already on top
		return;
	}
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *it = (guiWidget_t*)node->data;
		if (it == widget) {
			list_swap(container->children, node, container->children->head);
		}
	}
}
