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
	widget_getMinimalSize,
	container_showWidgetPart,
};

const char *ContainerType = "Container";

const char *container_typename(guiContainer *widget)
{
	return ContainerType;
}

guiContainer *container_new(guiGUI *gui)
{
	guiContainer *container = new (guiContainer);

	container_init(container);
	gui_addWidget(gui, container);

	return container;
}

void container_clear(guiContainer *container)
{
	for (listNode_t *node = container->children->head; node; node = node->next) {
		guiWidget *w = node->data;
		w->v->w_setFocusManager(w, NULL);
		widget_setParent(w, NULL);
		widget_removeListener(w, (guiEventListener *)container->listener);
	}
	list_clear(container->children);
}

void container_destructor(guiContainer *container)
{
	container_clear(container);

	// Destruction
	list_delete(container->children);
	free(container->listener);
	widget_destructor((guiWidget *)container);
}

void container_init(guiContainer *container)
{
	widget_init(&container->widget);
	container->widget.v = (guiWidget_vf_t *)&guiContainer_vtable;

	((guiWidget *)container)->isContainer = true;
	container->children = list_new();
	guiLifecycleListener *listener = lifecycleListener_new(container, LE_DESTROYED, container_death);
	container->listener = listener;
}

void container_death(void *widget, guiLifecycleEvent *event)
{
	guiContainer *container = (guiContainer *)widget;
	listNode_t *remove = list_find(container->children, event_getSource(event));
	if (!remove) {
		guiError("There is no such widget in this container.");
	}
	list_erase(container->children, remove);
}

void container_add(guiContainer *container, guiWidget *widget)
{
	list_push_back(container->children, (void *)widget);
	widget_setParent(widget, &container->widget);
	widget->v->w_setFocusManager(widget, widget_getFocusManager((guiWidget *)container));
	widget_addListener(widget, (guiEventListener *)container->listener);
}

void container_addAt(guiContainer *container, guiWidget *widget, vec2i pos)
{
	widget_setPosition(widget, pos);
	container_add(container, widget);
	guiInfo("container item added");
}

void container_remove(guiContainer *container, guiWidget *widget)
{
	for (listNode_t *node = container->children->head; node; node = node->next) {
		if ((guiWidget *)node->data == widget) {
			list_erase(container->children, node);
			widget->v->w_setFocusManager(widget, NULL);
			widget_setParent(widget, NULL);
			widget_removeListener(widget, (guiEventListener *)container->listener);
			return;
		}
	}
}

void container_draw(const guiContainer *container, guiGraphics *graphics)
{
	guiDebugPrint("drawing container");

	guiRectangle rect = *container->widget.v->w_getChildrenArea((guiWidget *)container);
	graph_pushClipArea(graphics, rect);
	ACS_SetHudSize(640, 480, 1);
	ACS_SetHudSize(graph_getScreenWidth(graphics), graph_getScreenHeight(graphics), 1);

	guiWidget *it ;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget *)node->data;
		if (widget_isVisible(it)) {
			graph_pushClipArea(graphics, widget_getDimensions(it));
			it->v->w_draw(it, graphics);
			graph_popClipArea(graphics);
		}
	}
	graph_popClipArea(graphics);
}

guiRectangle *container_getChildrenArea(const guiContainer *container)
{
	return &(guiRectangle) {0, 0, container->widget.dim.width, container->widget.dim.height};
}

guiWidget *container_getWidgetAt(const guiContainer *container, vec2i pos)
{
	guiRectangle *r = container->widget.v->w_getChildrenArea(&container->widget);

	if (!rect_isPointInRect(r, pos)) {
		return NULL;
	}

	vec_sub(pos, r->pos);
	guiWidget *it;
	guiRectangle *rect;

	for (listNode_t *node = container->children->tail; node; node = node->prev) {
		it = (guiWidget *)node->data;
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
	guiWidget *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget *)node->data;
		if (it->v->w_tick != widget_tick) {
			it->v->w_tick(it);
		}

	}
}

bool container_isWidgetExisting(guiContainer *widget, const guiWidget *exist)
{
	guiContainer *container = (guiContainer *)widget;
	guiWidget *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget *)node->data;
		if (it->v->w_isWidgetExisting(it, exist)) {
			return true;
		}
	}
	return false;
}

void container_setFocusManager(guiContainer *widget, void *focus)
{
	guiContainer *container = (guiContainer *)widget;
	widget_setFocusManager((guiWidget *)widget, focus);

	guiWidget *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget *)node->data;
		it->v->w_setFocusManager(it, focus);
	}
}

void container_showWidgetPart(guiContainer *container, guiWidget *widget, guiRectangle area)
{
	guiRectangle widgetArea = *container_getChildrenArea(container);
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

void container_moveToTop(guiContainer *container, guiWidget *widget)
{
	if (list_size(container->children) < 2) { // nothing in container or already on top
		return;
	}
	guiWidget *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget *)node->data;
		if (it == widget) {
			list_swap(container->children, node, container->children->tail);
		}
	}
}

void container_moveToBottom(guiContainer *container, guiWidget *widget)
{
	if (list_size(container->children) < 2) { // nothing in container or already on top
		return;
	}
	guiWidget *it;
	for (listNode_t *node = container->children->head; node; node = node->next) {
		it = (guiWidget *)node->data;
		if (it == widget) {
			list_swap(container->children, node, container->children->head);
		}
	}
}
