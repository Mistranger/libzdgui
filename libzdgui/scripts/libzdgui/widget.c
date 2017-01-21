#include "system.h"

#include "widget.h"

guiWidget_vf_t guiWidget_vtable = {
	widget_getChildrenArea,
	widget_getWidgetAt,
	widget_draw,
	widget_tick,
	widget_isWidgetExisting
};

void widget_tick(guiWidget_t *widget)
{
	return;
}

void widget_draw(const guiWidget_t *widget, guiGraphics_t *graphics)
{
	return;
}

void widget_init(guiWidget_t* widget)
{
	guiDebugPrint("trying to create widget");
	guiRectangle_t dim = {0, 0, 0, 0};
	widget->dim = dim;
	widget->flags = WF_ENABLED + WF_VISIBLE;
	widget->eventListeners = list_init();
	
	widget->v = &guiWidget_vtable;
	
	guiDebugPrint("new widget created");
}

guiRectangle_t widget_getChildrenArea(const guiWidget_t *widget)
{
	guiRectangle_t rect = {0, 0, 0, 0};
	return rect;
}

guiWidget_t* widget_getWidgetAt(const guiWidget_t* widget, int x, int y)
{
	return NULL;
}

bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist)
{
	return widget == exist ? true : false;
}

void widget_setWidth(guiWidget_t* widget, int width)
{
	guiRectangle_t newDimension = widget->dim;
	newDimension.width = width;
	widget_setDimension(widget, newDimension);
}

void widget_setHeight(guiWidget_t* widget, int height)
{
	guiRectangle_t newDimension = widget->dim;
	newDimension.height = height;
	widget_setDimension(widget, newDimension);
}

int widget_getWidth(guiWidget_t* widget)
{
	return widget->dim.width;
}

int widget_getHeight(guiWidget_t* widget)
{
	return widget->dim.height;
}

void widget_setX(guiWidget_t* widget, int x)
{
	guiRectangle_t newDimension = widget->dim;
	newDimension.x = x;
	widget_setDimension(widget, newDimension);
}

void widget_setY(guiWidget_t* widget, int y)
{
	guiRectangle_t newDimension = widget->dim;
	newDimension.y = y;
	widget_setDimension(widget, newDimension);
}

int widget_getX(guiWidget_t* widget)
{
	return widget->dim.x;
}

int widget_getY(guiWidget_t* widget)
{
	return widget->dim.y;
}

void widget_setPosition(guiWidget_t* widget, int x, int y)
{
	guiRectangle_t newDimension = widget->dim;
	newDimension.x = x;
	newDimension.y = y;
	widget_setDimension(widget, newDimension);
}

void widget_setSize(guiWidget_t* widget, int width, int height)
{
	guiRectangle_t newDimension = widget->dim;
	newDimension.width = width;
	newDimension.height = height;
	widget_setDimension(widget, newDimension);
}

guiRectangle_t widget_getDimensions(const guiWidget_t* widget)
{
	guiRectangle_t rect = widget->dim;
	return rect;
}

void widget_setDimension(guiWidget_t* widget, const guiRectangle_t newDim)
{
	guiRectangle_t oldDim = widget->dim;
	widget->dim = newDim;
	if (widget->dim.width != oldDim.width || widget->dim.height != oldDim.height) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Dimension;
		dimensionEvent_t *resized = (dimensionEvent_t*)malloc(sizeof(dimensionEvent_t));
		resized->x = newDim.x;
		resized->y = newDim.y;
		resized->width = newDim.width;
		resized->height = newDim.height;
		resized->type = DE_RESIZED;
		event->events.dimension = resized;
		widget_handleEvent((guiWidget_t*)widget, event);
	}
	if (widget->dim.x != oldDim.x || widget->dim.y != oldDim.y) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Dimension;
		dimensionEvent_t *moved = (dimensionEvent_t*)malloc(sizeof(dimensionEvent_t));
		moved->x = newDim.x;
		moved->y = newDim.y;
		moved->width = newDim.width;
		moved->height = newDim.height;
		moved->type = DE_MOVED;
		event->events.dimension = moved;
		widget_handleEvent((guiWidget_t*)widget, event);
	}
	guiDebugPrint("dimension set");
}

void widget_setParent(guiWidget_t* widget, guiWidget_t* newParent)
{
	widget->parent = newParent;
}

guiWidget_t* widget_getParent(guiWidget_t* widget)
{
	return widget->parent;
}

void widget_getAbsolutePosition(guiWidget_t* widget, int *x, int *y)
{
	if (!widget->parent){ 
		*x = widget->dim.x;
		*y = widget->dim.y;
		return;
	}

	int parentX;
	int parentY;

	widget_getAbsolutePosition(widget->parent, &parentX, &parentY);

	guiRectangle_t rect = widget->parent->v->w_getChildrenArea(widget->parent);
	*x = parentX + widget->dim.x + rect.x;
	*y = parentY + widget->dim.y + rect.y;
}

list_t* widget_getListeners(guiWidget_t* widget)
{
	return widget->eventListeners;
}

void widget_handleEvent(guiWidget_t* widget, event_t* event)
{
	for (listNode_t *node = widget->eventListeners->head; node; node = node->next) {
		eventListener_t *listener = (eventListener_t*)node->data;
		if (listener->type == event->type) {
			switch (listener->type) {
				case EV_Mouse: {
					mouseListener_t *mouseL = listener->listeners.mouseListener;
					mouseEvent_t *mouseE = event->events.mouse;
					
					if (mouseL->type == mouseE->type) {
						switch (mouseL->type) {
							case ME_MOVED:
								mouseL->types.mouseMoved((void*)widget, mouseE);
								break;
							case ME_PRESSED:
								mouseL->types.mousePressed((void*)widget, mouseE);
								break;
							case ME_RELEASED:
								mouseL->types.mouseReleased((void*)widget, mouseE);
								break;
							case ME_ENTERED:
								mouseL->types.mouseEntered((void*)widget, mouseE);
								break;
							case ME_LEFT:
								mouseL->types.mouseEntered((void*)widget, mouseE);
								break;
							case ME_CLICKED:
								mouseL->types.mouseClicked((void*)widget, mouseE);
								break;
						}
					}
				}
				break;
				case EV_Dimension: {
					dimensionListener_t *dimL = listener->listeners.dimensionListener;
					dimensionEvent_t *dimE = event->events.dimension;
					if (dimL->type == dimE->type) {
						switch (dimL->type) {
							case DE_MOVED:
								dimL->types.moved((void*)widget, dimE);
								break;
							case DE_RESIZED:
								dimL->types.resized((void*)widget, dimE);
								break;
						}
					}
				}
			}
		}
	}
}