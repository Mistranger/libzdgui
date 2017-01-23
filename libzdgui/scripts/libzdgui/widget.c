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
	widget->eventListeners = list_new();
	widget->font = NULL;
	
	widget->v = &guiWidget_vtable;
	
	guiDebugPrint("new widget created");
}

guiRectangle_t widget_getChildrenArea(const guiWidget_t *widget)
{
	guiRectangle_t rect = {0, 0, 0, 0};
	return rect;
}

guiWidget_t* widget_getWidgetAt(const guiWidget_t* widget, vec2i_t pos)
{
	return NULL;
}

bool widget_isWidgetExisting(guiWidget_t *widget, const guiWidget_t *exist)
{
	return widget == exist ? true : false;
}

void widget_setDimension(guiWidget_t* widget, const guiRectangle_t newDim)
{
	guiRectangle_t oldDim = widget->dim;
	widget->dim = newDim;
	if (widget->dim.width != oldDim.width || widget->dim.height != oldDim.height) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Dimension;
		dimensionEvent_t *resized = new(dimensionEvent_t);
		resized->pos = newDim.pos;
		resized->width = newDim.width;
		resized->height = newDim.height;
		resized->type = DE_RESIZED;
		event->events.dimension = resized;
		widget_handleEvent((guiWidget_t*)widget, event);
	}
	if (widget->dim.pos.x != oldDim.pos.x || widget->dim.pos.y != oldDim.pos.y) {
		event_t *event = (event_t*)malloc(sizeof(event_t));
		event->type = EV_Dimension;
		dimensionEvent_t *moved = new(dimensionEvent_t);
		moved->pos = newDim.pos;
		moved->width = newDim.width;
		moved->height = newDim.height;
		moved->type = DE_MOVED;
		event->events.dimension = moved;
		widget_handleEvent((guiWidget_t*)widget, event);
	}
	guiDebugPrint("dimension set");
}

void widget_getAbsolutePosition(guiWidget_t* widget, vec2i_t *pos)
{
	if (!widget->parent){ 
		pos->x = widget->dim.pos.x;
		pos->y = widget->dim.pos.y;
		return;
	}

	vec2i_t parentPos;

	widget_getAbsolutePosition(widget->parent, &parentPos);

	guiRectangle_t rect = widget->parent->v->w_getChildrenArea(widget->parent);
	pos->x = parentPos.x + widget->dim.pos.x + rect.pos.x;
	pos->y = parentPos.y + widget->dim.pos.y + rect.pos.y;
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
