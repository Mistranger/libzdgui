#include "system.h"

#include "widgets/container.h"

#include "dimension.h"
#include "util/list.h"

guiContainer_vf_t guiContainer_vtable = {
	container_typename,
	container_destructor,
	container_getChildrenArea,
	container_getWidgetAt,
	container_draw,
	container_tick,
	container_isWidgetExisting,
	container_setFocusManager,
	container_showWidgetPart
};

const char *ContainerType = "Container";

const char *container_typename(guiContainer *widget)
{
	return ContainerType;
}

guiContainer *container_new(guiGUI_t *gui)
{
	guiContainer *container = new (guiContainer);

	container_init(container);
	gui_addWidget(gui, container);

	return container;
}

void container_clear(guiContainer *container)
{
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget_t *w = node->data;
		w->v->w_setFocusManager(w, NULL);
		widget_setParent(w, NULL);
		widget_removeListener(w, (eventListener_t *)container->listener);
	}
	list_clear(container->children);
}

void container_destructor(guiContainer *container)
{
	container_clear(container);

	// Destruction
	list_delete(container->children);
	free(container->listener);
	widget_destructor((guiWidget_t *)container);
}

void container_init(guiContainer *container)
{
	widget_init(&container->widget);
	container->widget.v = (guiWidget_vf_t *)&guiContainer_vtable;

	((guiWidget_t *)container)->isContainer = true;
	container->children = list_new();
	lifecycleListener_t *listener = lifecycleListener_new(container, LE_DESTROYED, container_death);
	container->listener = listener;
}

void container_death(void *widget, lifecycleEvent_t *event)
{
	guiContainer *container = (guiContainer *)widget;
	listNode_t *remove = list_find(container->children, event_getSource(event));
	if (!remove) {
		guiError("There is no such widget in this container.");
	}
	list_erase(container->children, remove);
}

void container_add(guiContainer *container, guiWidget_t *widget)
{
	list_push_back(container->children, (void *)widget);
	widget_setParent(widget, &container->widget);
	widget->v->w_setFocusManager(widget, widget_getFocusManager((guiWidget_t *)container));
	widget_addListener(widget, (eventListener_t *)container->listener);
}

void container_addAt(guiContainer *container, guiWidget_t *widget, vec2i_t pos)
{
	widget_setPosition(widget, pos);
	container_add(container, widget);
	guiInfo("container item added");
}

void container_remove(guiContainer *container, guiWidget_t *widget)
{
	for (listNode_t *node = container->children->head; node; node = node->next) {
		if ((guiWidget_t *)node->data == widget) {
			list_erase(container->children, node);
			widget->v->w_setFocusManager(widget, NULL);
			widget_setParent(widget, NULL);
			widget_removeListener(widget, (eventListener_t *)container->listener);
			return;
		}
	}
}

void container_draw(const guiContainer *container, guiGraphics_t *graphics)
{
	guiDebugPrint("drawing container");

	guiRectangle_t rect = *container->widget.v->w_getChildrenArea((guiWidget_t *)container);
	graph_pushClipArea(graphics, rect);
	ACS_SetHudSize(640, 480, 1);
	ACS_SetHudSize(graph_getScreenWidth(graphics), graph_getScreenHeight(graphics), 1);

	guiWidget_t *it ;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget_t *)node->data;
		if (widget_isVisible(it)) {
			graph_pushClipArea(graphics, widget_getDimensions(it));
			it->v->w_draw(it, graphics);
			graph_popClipArea(graphics);
		}
	}
	graph_popClipArea(graphics);
}

guiRectangle_t *container_getChildrenArea(const guiContainer *container)
{
	return &(guiRectangle_t) {0, 0, container->widget.dim.width, container->widget.dim.height};
}

guiWidget_t *container_getWidgetAt(const guiContainer *container, vec2i_t pos)
{
	guiRectangle_t *r = container->widget.v->w_getChildrenArea(&container->widget);

	if (!rect_isPointInRect(r, pos)) {
		return NULL;
	}

	vec_sub(pos, r->pos);
	guiWidget_t *it;
	guiRectangle_t *rect;

	for (listNode_t *node = container->children->tail; node; node = node->prev) {
		it = (guiWidget_t *)node->data;
		rect = &widget_getDimensions(it);
		if (widget_isVisible(it) && rect_isPointInRect(rect, pos)) {
			return it;
		}
	}

	return NULL;
}

void container_tick(guiContainer *widget)
{
	guiDebugPrint("container wtick");
	guiContainer *container = (guiContainer *)widget;
	guiWidget_t *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget_t *)node->data;
		if (it->v->w_tick != widget_tick) {
			it->v->w_tick(it);
		}

	}
}

bool container_isWidgetExisting(guiContainer *widget, const guiWidget_t *exist)
{
	guiContainer *container = (guiContainer *)widget;
	guiWidget_t *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget_t *)node->data;
		if (it->v->w_isWidgetExisting(it, exist)) {
			return true;
		}
	}
	return false;
}

void container_setFocusManager(guiContainer *widget, void *focus)
{
	guiContainer *container = (guiContainer *)widget;
	widget_setFocusManager((guiWidget_t *)widget, focus);

	guiWidget_t *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget_t *)node->data;
		it->v->w_setFocusManager(it, focus);
	}
}

void container_showWidgetPart(guiContainer *container, guiWidget_t *widget, guiRectangle_t area)
{
	guiRectangle_t widgetArea = *container_getChildrenArea(container);
	area.pos.x += widget_getX(widget);
	area.pos.y += widget_getY(widget);

	if (area.pos.x + area.width > widgetArea.width) {
		widget_setX(widget, widget_getX(widget) - area.pos.x - area.width + widgetArea.width);
	}

	if (area.pos.y + area.height > widgetArea.height) {
		widget_setY(widget, widget_getY(widget) - area.pos.y - area.height + widgetArea.height);
	}

	if (area.pos.x < 0) {
		widget_setX(widget, widget_getX(widget) - area.pos.x);
	}

	if (area.pos.y < 0) {
		widget_setY(widget, widget_getY(widget) - area.pos.y);
	}
}

void container_moveToTop(guiContainer *container, guiWidget_t *widget)
{
	if (list_size(container->children) < 2) { // nothing in container or already on top
		return;
	}
	guiWidget_t *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget_t *)node->data;
		if (it == widget) {
			list_swap(container->children, node, container->children->tail);
		}
	}
}

void container_moveToBottom(guiContainer *container, guiWidget_t *widget)
{
	if (list_size(container->children) < 2) { // nothing in container or already on top
		return;
	}
	guiWidget_t *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget_t *)node->data;
		if (it == widget) {
			list_swap(container->children, node, container->children->head);
		}
	}
}
