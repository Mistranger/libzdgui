#include "system.h"

#include "widgets/scrollarea.h"

guiScrollArea_vf_t guiScrollArea_vtable = {
	scroll_typename,
	scroll_destructor,
	scroll_getChildrenArea,
	scroll_getWidgetAt,
	scroll_draw,
	scroll_tick,
	container_isWidgetExisting,
	container_setFocusManager,
	widget_getMinimalSize,
	scroll_showWidgetPart,
};

guiImage defScrollVert = {13, 170, s"SLIDVERT"};
guiImage defScrollKnob = {20, 20, s"SLIDKNOB"};
guiImage defScrollDownPressed ={20, 20, s"SCDOWNPR"};
guiImage defScrollDown = {20, 20, s"SCDOWN"};
guiImage defScrollUpPressed = {20, 20, s"SCUPPR"};
guiImage defScrollUp = {20, 20, s"SCUP"};
guiImage defScrollLeft = {20, 20, s"SCLEFT"};
guiImage defScrollLeftPressed = {20, 20, s"SCLEFTPR"};
guiImage defScrollRightPressed = {20, 20, s"SCRITEPR"};
guiImage defScrollRight = {20, 20, s"SCRITE"};
guiImage defSliderHoriz = {170, 13, s"SLIDHORZ"};

const char *ScrollAreaType = "ScrollArea";

const char *scroll_typename(guiScrollArea *widget)
{
	return ScrollAreaType;
}

guiScrollArea *scroll_new(guiGUI *gui, guiWidget *content)
{
	guiScrollArea *scrollarea = new (guiScrollArea);
	scroll_init(scrollarea, content);
	gui_addWidget(gui, scrollarea);
	return scrollarea;
}

void scroll_init(guiScrollArea *scrollarea, guiWidget *content)
{
	container_init(&scrollarea->widget);
	scrollarea->widget.widget.v = (guiWidget_vf_t *)&guiScrollArea_vtable;

	scrollarea->upButtonImage = &defScrollUp;
	scrollarea->upPressedButtonImage = &defScrollUpPressed;
	scrollarea->downButtonImage = &defScrollDown;
	scrollarea->downPressedButtonImage = &defScrollDownPressed;
	scrollarea->leftButtonImage = &defScrollLeft;
	scrollarea->leftPressedButtonImage = &defScrollLeftPressed;
	scrollarea->rightButtonImage = &defScrollRight;
	scrollarea->rightPressedButtonImage = &defScrollRightPressed;
	scrollarea->hBarImage = &defSliderHoriz;
	scrollarea->vBarImage = &defScrollVert;
	scrollarea->markerImage = &defScrollKnob;

	scrollarea->scrollPos = (vec2i) { 0, 0 };
	scrollarea->scrollBarWidth = 20;
	scrollarea->hPolicy = SAP_AUTO;
	scrollarea->vPolicy = SAP_AUTO;

	scrollarea->upButtonPressed = false;
	scrollarea->downButtonPressed = false;
	scrollarea->leftButtonPressed = false;
	scrollarea->rightButtonPressed = false;
	scrollarea->upButtonScrollAmount = 10;
	scrollarea->downButtonScrollAmount = 10;
	scrollarea->leftButtonScrollAmount = 10;
	scrollarea->rightButtonScrollAmount = 10;
	scrollarea->isVerticalMarkerDragged = false;
	scrollarea->isHorizontalMarkerDragged = false;

	scroll_setContent(scrollarea, content);

	widget_addMouseListener(&scrollarea->widget, ME_PRESSED, scroll_mousePressed);
	widget_addMouseListener(&scrollarea->widget, ME_DRAGGED, scroll_mouseDragged);
	widget_addMouseListener(&scrollarea->widget, ME_RELEASED, scroll_mouseReleased);

	scrollarea->dimListener = dimensionListener_new(scrollarea, DE_RESIZED, scroll_resized);
	scrollarea->lifecycleListener = lifecycleListener_new(scrollarea, LE_DESTROYED, scroll_death);
	widget_addListener(&scrollarea->widget, scrollarea->dimListener);
}

void scroll_destructor(guiScrollArea *scrollarea)
{
	scroll_setContent(scrollarea, NULL);
	free(scrollarea->dimListener);
	free(scrollarea->lifecycleListener);
	container_destructor((guiContainer *)scrollarea);
}

void scroll_death(void *widget, guiLifecycleEvent *event)
{
	guiScrollArea *scrollarea = (guiScrollArea *)widget;
	listNode_t *remove = list_find(((guiContainer *)scrollarea)->children, event_getSource(event));
	if (!remove) {
		guiError("There is no such widget in this container.");
	}
	list_erase(((guiContainer *)scrollarea)->children, remove);
}

void scroll_checkPolicies(guiScrollArea *scrollarea)
{
	if (!scroll_getContent(scrollarea)) {
		scrollarea->hBarVisible = (scrollarea->hPolicy == SAP_ALWAYS);
		scrollarea->vBarVisible = (scrollarea->vPolicy == SAP_ALWAYS);
		return;
	}

	int w = widget_getWidth(scrollarea);
	int h = widget_getHeight(scrollarea);
	guiWidget *content = scroll_getContent(scrollarea);

	scrollarea->hBarVisible = false;
	scrollarea->vBarVisible = false;

	if (scrollarea->hPolicy == SAP_AUTO && scrollarea->vPolicy == SAP_AUTO) {
		if (widget_getWidth(content) <= w && widget_getHeight(content) <= h) {
			scrollarea->hBarVisible = false;
			scrollarea->vBarVisible = false;
		}
		if (widget_getWidth(content) > w) {
			scrollarea->hBarVisible = true;
		}
		if ((widget_getHeight(content) > h)
			|| (scrollarea->hBarVisible && widget_getHeight(content) > h - scrollarea->scrollBarWidth)) {
			scrollarea->vBarVisible = true;
		}
		if (scrollarea->vBarVisible && widget_getWidth(content) > w - scrollarea->scrollBarWidth) {
			scrollarea->hBarVisible = true;
		}
		return;
	}

	switch (scrollarea->hPolicy) {
		case SAP_NEVER:
			scrollarea->hBarVisible = false;
			break;
		case SAP_ALWAYS:
			scrollarea->hBarVisible = true;
			break;
		case SAP_AUTO:
			if (scrollarea->vPolicy == SAP_NEVER) {
				scrollarea->hBarVisible = widget_getWidth(content) > w;
			} else {
				scrollarea->hBarVisible = widget_getWidth(content) > w - scrollarea->scrollBarWidth;
			}
			break;
		default:
			guiError("Horizontal scroll policy invalid.");
	}

	switch (scrollarea->vPolicy) {
		case SAP_NEVER:
			scrollarea->vBarVisible = false;
			break;
		case SAP_ALWAYS:
			scrollarea->vBarVisible = true;
			break;
		case SAP_AUTO:
			if (scrollarea->hPolicy == SAP_NEVER) {
				scrollarea->vBarVisible = widget_getHeight(content) > h;
			} else {
				scrollarea->vBarVisible = widget_getHeight(content) > h - scrollarea->scrollBarWidth;
			}
			break;
		default:
			guiError("Vertical scroll policy invalid.");
	}
}

void scroll_setContent(guiScrollArea *scrollarea, guiWidget *content)
{
	if (content) {
		container_add((guiContainer *)scrollarea, content);
		widget_setPosition(scrollarea, ((vec2i) {0, 0}));
		widget_addListener(content, scrollarea->dimListener);
	} else {
		if (list_size(((guiContainer *)scrollarea)->children)) {
			guiWidget *w = (guiWidget*)list_front(((guiContainer *)scrollarea)->children)->data;
			widget_removeListener(w, scrollarea->dimListener);
			widget_removeListener(w, scrollarea->lifecycleListener);
		}
		
		container_clear((guiContainer *)scrollarea);
	}

	scroll_checkPolicies(scrollarea);
}

int scroll_getHorizontalMaxScroll(const guiScrollArea *scrollarea)
{
	if (!scroll_getContent(scrollarea)) {
		return 0;
	}

	guiWidget *content = scroll_getContent(scrollarea);
	int value = widget_getWidth(content) - scroll_getChildrenArea(scrollarea)->width;
	return value < 0 ? 0 : value;
}

int scroll_getVerticalMaxScroll(const guiScrollArea *scrollarea)
{
	if (!scroll_getContent(scrollarea)) {
		return 0;
	}

	guiWidget *content = scroll_getContent(scrollarea);
	int value = widget_getHeight(content) - scroll_getChildrenArea(scrollarea)->height;
	return value < 0 ? 0 : value;
}

guiWidget *scroll_getWidgetAt(const guiScrollArea *scrollarea, vec2i pos)
{
	if (rect_isPointInRect(scroll_getChildrenArea(scrollarea), pos)) {
		return scroll_getContent(scrollarea);
	}

	return NULL;
}

void scroll_showWidgetPart(guiScrollArea *scrollarea, guiWidget *widget, guiRectangle area)
{
	if (scroll_getContent(scrollarea) != widget) {
		guiWarning("Widget not content widget");
	}

	container_showWidgetPart((guiContainer *)scrollarea, widget, area);

	scroll_setHorizontalScrollAmount(scrollarea, -widget_getX(scroll_getContent(scrollarea)));
	scroll_setVerticalScrollAmount(scrollarea, -widget_getY(scroll_getContent(scrollarea)));
}

guiRectangle *scroll_getChildrenArea(const guiScrollArea *scrollarea)
{
	if (scrollarea->vBarVisible && scrollarea->hBarVisible) {
		return &(guiRectangle) {
			0, 0, widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
			widget_getHeight(scrollarea) - scrollarea->scrollBarWidth
		};
	}
	if (scrollarea->vBarVisible) {
		return &(guiRectangle) {
			0, 0, widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
			widget_getHeight(scrollarea)
		};
	}

	if (scrollarea->hBarVisible) {
		return &(guiRectangle) {
			0, 0, widget_getWidth(scrollarea),
			widget_getHeight(scrollarea) - scrollarea->scrollBarWidth
		};
	}
	return &(guiRectangle) {
		0, 0, widget_getWidth(scrollarea),
		widget_getHeight(scrollarea)
	};
}

guiRectangle *scroll_getContentDimension(const guiScrollArea *scrollarea)
{
	if (scrollarea->vBarVisible && scrollarea->hBarVisible) {
		return &(guiRectangle) {
			0, 0, widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
			widget_getHeight(scrollarea) - scrollarea->scrollBarWidth
		};
	}

	if (scrollarea->vBarVisible) {
		return &(guiRectangle) {0, 0, widget_getWidth(scrollarea) - scrollarea->scrollBarWidth, widget_getHeight(scrollarea)};
	}

	if (scrollarea->hBarVisible) {
		return &(guiRectangle) {
			0, 0, widget_getWidth(scrollarea), widget_getHeight(scrollarea) - scrollarea->scrollBarWidth
		};
	}

	return &(guiRectangle) {
		0, 0, widget_getWidth(scrollarea),
		widget_getHeight(scrollarea)
	};
}

guiRectangle *scroll_getVerticalBarDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->vBarVisible) {
		return &(guiRectangle) {0, 0, 0, 0};
	}

	if (scrollarea->hBarVisible) {
		return &(guiRectangle) {
			widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
							(scroll_getUpButtonDimension(scrollarea))->height,
							scrollarea->scrollBarWidth,
							widget_getHeight(scrollarea)
							- (scroll_getUpButtonDimension(scrollarea))->height
							- (scroll_getDownButtonDimension(scrollarea))->height
							- scrollarea->scrollBarWidth
		};
	}

	return &(guiRectangle) {
		widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
						(scroll_getUpButtonDimension(scrollarea))->height,
						scrollarea->scrollBarWidth,
						widget_getHeight(scrollarea)
						- (scroll_getUpButtonDimension(scrollarea))->height
						- (scroll_getDownButtonDimension(scrollarea))->height
	};
}

guiRectangle *scroll_getHorizontalBarDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->hBarVisible) {
		return &(guiRectangle) {0, 0, 0, 0};
	}

	if (scrollarea->vBarVisible) {
		return &(guiRectangle) {
			(scroll_getLeftButtonDimension(scrollarea))->width,
			widget_getHeight(scrollarea) - scrollarea->scrollBarWidth,
			widget_getWidth(scrollarea)
			- (scroll_getLeftButtonDimension(scrollarea))->width
			- (scroll_getRightButtonDimension(scrollarea))->width
			- scrollarea->scrollBarWidth,
			scrollarea->scrollBarWidth
		};
	}

	return &(guiRectangle) {
		(scroll_getLeftButtonDimension(scrollarea))->width,
		widget_getHeight(scrollarea) - scrollarea->scrollBarWidth,
		widget_getWidth(scrollarea)
		- (scroll_getLeftButtonDimension(scrollarea))->width
		- (scroll_getRightButtonDimension(scrollarea))->width,
		scrollarea->scrollBarWidth
	};
}

guiRectangle *scroll_getUpButtonDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->vBarVisible) {
		return &(guiRectangle) {0, 0, 0, 0};
	}

	return &(guiRectangle) {
		widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
						0,
						scrollarea->scrollBarWidth,
						scrollarea->scrollBarWidth
	};
}

guiRectangle *scroll_getDownButtonDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->vBarVisible) {
		return &(guiRectangle) {0, 0, 0, 0};
	}

	if (scrollarea->vBarVisible && scrollarea->hBarVisible)	{
		return &(guiRectangle) {
			widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
							widget_getHeight(scrollarea) - scrollarea->scrollBarWidth * 2,
							scrollarea->scrollBarWidth,
							scrollarea->scrollBarWidth
		};
	}

	return &(guiRectangle) {
		widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
						widget_getHeight(scrollarea) - scrollarea->scrollBarWidth,
						scrollarea->scrollBarWidth,
						scrollarea->scrollBarWidth
	};
}

guiRectangle *scroll_getLeftButtonDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->hBarVisible) {
		return &(guiRectangle) {0, 0, 0, 0};
	}

	return &(guiRectangle) {
		0,
		widget_getHeight(scrollarea) - scrollarea->scrollBarWidth,
		scrollarea->scrollBarWidth,
		scrollarea->scrollBarWidth
	};
}

guiRectangle *scroll_getRightButtonDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->hBarVisible) {
		return &(guiRectangle) {0, 0, 0, 0};
	}

	if (scrollarea->vBarVisible && scrollarea->hBarVisible) {
		return &(guiRectangle) {
			widget_getWidth(scrollarea) - scrollarea->scrollBarWidth * 2,
							widget_getHeight(scrollarea) - scrollarea->scrollBarWidth,
							scrollarea->scrollBarWidth,
							scrollarea->scrollBarWidth
		};
	}

	return &(guiRectangle) {
		widget_getWidth(scrollarea) - scrollarea->scrollBarWidth,
						widget_getHeight(scrollarea) - scrollarea->scrollBarWidth,
						scrollarea->scrollBarWidth,
						scrollarea->scrollBarWidth
	};
}

guiRectangle *scroll_getVerticalMarkerDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->vBarVisible) {
		return &(guiRectangle) {0, 0, 0, 0};
	}

	int length, pos;
	guiRectangle barDim = *scroll_getVerticalBarDimension(scrollarea);
	guiWidget *content = scroll_getContent(scrollarea);

	if (content && widget_getHeight(content) != 0)	{
		length = image_getHeight(*scrollarea->markerImage);
	} else {
		length = barDim.height;
	}
	if (length < scrollarea->scrollBarWidth) {
		length = scrollarea->scrollBarWidth;
	}
	if (length > barDim.height)	{
		length = barDim.height;
	}

	if (scroll_getVerticalMaxScroll(scrollarea) != 0) {
		pos = ((barDim.height - length) * scroll_getVerticalScrollAmount(scrollarea))
			  / scroll_getVerticalMaxScroll(scrollarea);
	} else {
		pos = 0;
	}

	return &(guiRectangle) {barDim.pos.x, barDim.pos.y + pos, scrollarea->scrollBarWidth, length};
}

guiRectangle *scroll_getHorizontalMarkerDimension(const guiScrollArea *scrollarea)
{
	if (!scrollarea->hBarVisible) {
		return &(guiRectangle) { 0, 0, 0, 0 };
	}

	int length, pos;
	guiRectangle barDim = *scroll_getHorizontalBarDimension(scrollarea);
	guiWidget *content = scroll_getContent(scrollarea);

	if (content && widget_getWidth(content) != 0) {
		length = image_getWidth(*scrollarea->markerImage);
	} else {
		length = barDim.width;
	}

	if (length < scrollarea->scrollBarWidth) {
		length = scrollarea->scrollBarWidth;
	}

	if (length > barDim.width) {
		length = barDim.width;
	}

	if (scroll_getHorizontalMaxScroll(scrollarea) != 0) {
		pos = ((barDim.width - length) * scroll_getHorizontalScrollAmount(scrollarea))
			  / scroll_getHorizontalMaxScroll(scrollarea);
	} else {
		pos = 0;
	}

	return &(guiRectangle) {
		barDim.pos.x + pos, barDim.pos.y, length, scrollarea->scrollBarWidth
	};
}

static void scroll_drawHBar(const guiScrollArea *scrollarea, guiGraphics *graphics)
{
	guiRectangle dim = *scroll_getHorizontalBarDimension(scrollarea);
	graph_pushClipArea(graphics, dim);
	graph_drawImageScaled(graphics, 0, 0,
						  image_getWidth(*scrollarea->hBarImage),
						  image_getHeight(*scrollarea->hBarImage), dim.width, dim.height, image_getImage(*scrollarea->hBarImage));

	graph_popClipArea(graphics);
}

static void scroll_drawVBar(const guiScrollArea *scrollarea, guiGraphics *graphics)
{
	guiRectangle dim = *scroll_getVerticalBarDimension(scrollarea);

	graph_pushClipArea(graphics, dim);
	graph_drawImageScaled(graphics, 0, 0,
						  image_getWidth(*scrollarea->vBarImage),
						  image_getHeight(*scrollarea->vBarImage), dim.width, dim.height, image_getImage(*scrollarea->vBarImage));

	graph_popClipArea(graphics);
}

void scroll_draw(const guiScrollArea *scrollarea, guiGraphics *graphics)
{
	// Check if we have all required graphics
	if (!scrollarea->upButtonImage || !scrollarea->downButtonImage || !scrollarea->leftButtonImage
		|| !scrollarea->rightButtonImage || !scrollarea->upPressedButtonImage
		|| !scrollarea->downPressedButtonImage || !scrollarea->leftPressedButtonImage
		|| !scrollarea->rightPressedButtonImage 	|| !scrollarea->markerImage
		|| !scrollarea->hBarImage || !scrollarea->vBarImage) {
		guiError("Not all graphics for ImageListBoxWidget were set");
		return;
	}
	guiDebugPrint("drawing scrollarea");
	guiRectangle dim;

	if (scrollarea->vBarVisible) {
		if (scrollarea->upButtonPressed) {
			dim = *scroll_getUpButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->upPressedButtonImage),
								  image_getHeight(*scrollarea->upPressedButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->upPressedButtonImage));
		} else {
			dim = *scroll_getUpButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->upButtonImage),
								  image_getHeight(*scrollarea->upButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->upButtonImage));
		}
		if (scrollarea->downButtonPressed) {
			dim = *scroll_getDownButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->downPressedButtonImage),
								  image_getHeight(*scrollarea->downPressedButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->downPressedButtonImage));
		} else {
			dim = *scroll_getDownButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->downButtonImage),
								  image_getHeight(*scrollarea->downButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->downButtonImage));
		}
		scroll_drawVBar(scrollarea, graphics);
		dim = *scroll_getVerticalMarkerDimension(scrollarea);
		graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
							  image_getWidth(*scrollarea->markerImage),
							  image_getHeight(*scrollarea->markerImage),
							  dim.width, dim.height,
							  image_getImage(*scrollarea->markerImage));
	}
	if (scrollarea->hBarVisible) {
		if (scrollarea->leftButtonPressed) {
			dim = *scroll_getLeftButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->leftPressedButtonImage),
								  image_getHeight(*scrollarea->leftPressedButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->leftPressedButtonImage));
		} else {
			dim = *scroll_getLeftButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->leftButtonImage),
								  image_getHeight(*scrollarea->leftButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->leftButtonImage));
		}
		if (scrollarea->rightButtonPressed) {
			dim = *scroll_getRightButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->rightPressedButtonImage),
								  image_getHeight(*scrollarea->rightPressedButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->rightPressedButtonImage));
		} else {
			dim = *scroll_getRightButtonDimension(scrollarea);
			graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
								  image_getWidth(*scrollarea->rightButtonImage),
								  image_getHeight(*scrollarea->rightButtonImage),
								  dim.width, dim.height,
								  image_getImage(*scrollarea->rightButtonImage));
		}
		scroll_drawHBar(scrollarea, graphics);
		dim = *scroll_getHorizontalMarkerDimension(scrollarea);
		graph_drawImageScaled(graphics, dim.pos.x, dim.pos.y,
							  image_getWidth(*scrollarea->markerImage),
							  image_getHeight(*scrollarea->markerImage),
							  dim.width, dim.height,
							  image_getImage(*scrollarea->markerImage));
	}
	guiWidget *content = scroll_getContent(scrollarea);
	if (content && (widget_isVisible(content))) {
		guiRectangle contdim = widget_getDimensions(content);
		graph_pushClipArea(graphics, *scroll_getContentDimension(scrollarea));
		graph_pushClipArea(graphics, contdim);
		content->v->w_draw(content, graphics);
		graph_popClipArea(graphics);
		graph_popClipArea(graphics);
	}
}

void scroll_tick(guiScrollArea *scrollarea)
{
	scroll_setVerticalScrollAmount(scrollarea, scroll_getVerticalScrollAmount(scrollarea));
	scroll_setHorizontalScrollAmount(scrollarea, scroll_getHorizontalScrollAmount(scrollarea));
	if (scroll_getContent(scrollarea) != NULL) {
		vec2i pos = {-scrollarea->scrollPos.x, -scrollarea->scrollPos.y};
		widget_setPosition(scroll_getContent(scrollarea), pos);
		scroll_getContent(scrollarea)->v->w_tick(scroll_getContent(scrollarea));
	}
}

void scroll_mousePressed(void *widget, guiMouseEvent *mouseEvent)
{
	guiScrollArea *scrollarea = (guiScrollArea *)widget;

	if (rect_isPointInRect(scroll_getUpButtonDimension(scrollarea), mouseEvent->pos)) {
		scroll_setVerticalScrollAmount(scrollarea, scroll_getVerticalScrollAmount(scrollarea)
			- scrollarea->upButtonScrollAmount);
		scrollarea->upButtonPressed = true;
	} else if (rect_isPointInRect(scroll_getDownButtonDimension(scrollarea), mouseEvent->pos)) {
		scroll_setVerticalScrollAmount(scrollarea, scroll_getVerticalScrollAmount(scrollarea)
									   + scrollarea->downButtonScrollAmount);
		scrollarea->downButtonPressed = true;
	} else if (rect_isPointInRect(scroll_getLeftButtonDimension(scrollarea), mouseEvent->pos)) {
		scroll_setHorizontalScrollAmount(scrollarea, scroll_getHorizontalScrollAmount(scrollarea)
										 - scrollarea->leftButtonScrollAmount);
		scrollarea->leftButtonPressed = true;
	} else if (rect_isPointInRect(scroll_getRightButtonDimension(scrollarea), mouseEvent->pos))	{
		scroll_setHorizontalScrollAmount(scrollarea, scroll_getHorizontalScrollAmount(scrollarea)
										 + scrollarea->rightButtonScrollAmount);
		scrollarea->rightButtonPressed = true;
	} else if (rect_isPointInRect(scroll_getVerticalMarkerDimension(scrollarea), mouseEvent->pos)) {
		scrollarea->isHorizontalMarkerDragged = false;
		scrollarea->isVerticalMarkerDragged = true;

		scrollarea->verticalMarkerDragOffset = mouseEvent->pos.y - scroll_getVerticalMarkerDimension(scrollarea)->pos.y;
	} else if (rect_isPointInRect(scroll_getVerticalBarDimension(scrollarea), mouseEvent->pos)) {
		if (mouseEvent->pos.y < scroll_getVerticalMarkerDimension(scrollarea)->pos.y) {
			scroll_setVerticalScrollAmount(scrollarea, scroll_getVerticalScrollAmount(scrollarea)
										   - (int)(scroll_getChildrenArea(scrollarea)->height * 0.95));
		} else {
			scroll_setVerticalScrollAmount(scrollarea, scroll_getVerticalScrollAmount(scrollarea)
										   + (int)(scroll_getChildrenArea(scrollarea)->height * 0.95));
		}
	} else if (rect_isPointInRect(scroll_getHorizontalMarkerDimension(scrollarea), mouseEvent->pos)) {
		scrollarea->isHorizontalMarkerDragged = true;
		scrollarea->isVerticalMarkerDragged = false;

		scrollarea->horizontalMarkerDragOffset = mouseEvent->pos.x
												 - scroll_getHorizontalMarkerDimension(scrollarea)->pos.x;
	} else if (rect_isPointInRect(scroll_getHorizontalBarDimension(scrollarea), mouseEvent->pos)) {
		if (mouseEvent->pos.x < scroll_getHorizontalMarkerDimension(scrollarea)->pos.x) {
			scroll_setHorizontalScrollAmount(scrollarea, scroll_getHorizontalScrollAmount(scrollarea)
											 - (int)(scroll_getChildrenArea(scrollarea)->width * 0.95));
		} else {
			scroll_setHorizontalScrollAmount(scrollarea, scroll_getHorizontalScrollAmount(scrollarea)
											 + (int)(scroll_getChildrenArea(scrollarea)->width * 0.95));
		}
	}
}

void scroll_mouseDragged(void *widget, guiMouseEvent *mouseEvent)
{
	guiScrollArea *scrollarea = (guiScrollArea *)widget;
	if (scrollarea->isVerticalMarkerDragged) {
		int pos = mouseEvent->pos.y - scroll_getVerticalBarDimension(scrollarea)->pos.y - scrollarea->verticalMarkerDragOffset;
		int length = scroll_getVerticalMarkerDimension(scrollarea)->height;

		guiRectangle barDim = *scroll_getVerticalBarDimension(scrollarea);

		if ((barDim.height - length) > 0) {
			scroll_setVerticalScrollAmount(scrollarea, (scroll_getVerticalMaxScroll(scrollarea) * pos) / (barDim.height - length));
		} else {
			scroll_setVerticalScrollAmount(scrollarea, 0);
		}
	}

	if (scrollarea->isHorizontalMarkerDragged) {
		int pos = mouseEvent->pos.x - scroll_getHorizontalBarDimension(scrollarea)->pos.x - scrollarea->horizontalMarkerDragOffset;
		int length = scroll_getHorizontalMarkerDimension(scrollarea)->width;

		guiRectangle barDim = *scroll_getHorizontalBarDimension(scrollarea);

		if ((barDim.width - length) > 0) {
			scroll_setHorizontalScrollAmount(scrollarea, (scroll_getHorizontalMaxScroll(scrollarea) * pos) / (barDim.width - length));
		} else {
			scroll_setHorizontalScrollAmount(scrollarea, 0);
		}
	}
}

void scroll_mouseReleased(void *widget, guiMouseEvent *mouseEvent)
{
	guiScrollArea *scrollarea = (guiScrollArea *)widget;
	scrollarea->upButtonPressed = false;
	scrollarea->downButtonPressed = false;
	scrollarea->leftButtonPressed = false;
	scrollarea->rightButtonPressed = false;
	scrollarea->isHorizontalMarkerDragged = false;
	scrollarea->isVerticalMarkerDragged = false;
}

void scroll_resized(void *widget, guiDimensionEvent *dimensionEvent)
{
	scroll_checkPolicies((guiScrollArea *)widget);
}

