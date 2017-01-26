#include "system.h"

#include "widgets/slider.h"
#include "util/util.h"

guiSlider_vf_t guiSlider_vtable = {
	slider_typename,
	widget_destructor,
	widget_getChildrenArea,
	widget_getWidgetAt,
	slider_draw,
	widget_tick,
	widget_isWidgetExisting
};

const char *SliderType = "Slider";

const char* slider_typename(guiSlider_t *widget)
{
	return SliderType;
}

guiSlider_t* slider_new(const int maxScale)
{
	guiSlider_t *slider = new(guiSlider_t);
	slider_init(slider, maxScale);

	return slider;
}

void slider_init(guiSlider_t* slider, const int maxScale)
{
	widget_init(&slider->widget);

	slider->widget.v = (guiWidget_vf_t*)&guiSlider_vtable;

	slider->markerImage = NULL;
	slider->backgroundImage = NULL;
	slider->disabledBackgroundImage = NULL;
	slider->end = maxScale;
	slider->start = 0;
	slider_setOrientation(slider, SLIDER_HORIZONTAL);
	slider_setStepLength(slider, (slider->end - slider->start) / 10);
	slider_setMarkerLength(slider, 10);

	widget_addMouseListener(&slider->widget, ME_PRESSED, slider_mousePressed);
}

static void slider_drawMarker(const guiSlider_t* slider, guiGraphics_t* graphics)
{
	guiImage_t *img = slider->markerImage;

	if (widget_isEnabled(slider)) {
		if (img) {
			if (slider_getOrientation(slider) == SLIDER_HORIZONTAL) {
				graph_drawImageScaled(graphics, slider_getMarkerPosition(slider), 0, image_getWidth(*img), image_getHeight(*img),
					image_getWidth(*img), widget_getHeight(slider), image_getImage(*img));
			} else {
				graph_drawImageScaled(graphics, 0, 
					(widget_getHeight(slider) - slider_getMarkerLength(slider)) - slider_getMarkerPosition(slider),
					image_getWidth(*img), image_getHeight(*img),
					widget_getHeight(slider), image_getHeight(*img), image_getImage(*img));
			}
		}
	}
}

void slider_draw(const guiSlider_t* slider, guiGraphics_t* graphics)
{
	guiImage_t *img;
	if (widget_isEnabled(slider)) {
		img = slider->backgroundImage;
	} else {
		img = slider->disabledBackgroundImage;
	}
	
	if (img) {
		graph_drawImageScaled(graphics, 0, 0, image_getWidth(*img), image_getHeight(*img),
			widget_getWidth(slider), widget_getHeight(slider), image_getImage(*img));
		if (widget_isEnabled(slider)) {
			slider_drawMarker(slider, graphics);
		}
	}
}

int slider_markerPositionToValue(const guiSlider_t* slider, int v)
{
	int w;
	if (slider_getOrientation(slider) == SLIDER_HORIZONTAL) {
		w = widget_getWidth(slider);
	} else {
		w = widget_getHeight(slider);
	}

	int pos = v * 100 / (w - slider_getMarkerLength(slider));
	return ((100 - pos) * slider_getScaleStart(slider) + pos * slider_getScaleEnd(slider)) / 100;
}

int slider_valueToMarkerPosition(const guiSlider_t* slider, int value)
{
	int v;
	if (slider_getOrientation(slider) == SLIDER_HORIZONTAL) {
		v = widget_getWidth(slider);
	} else {
		v = widget_getHeight(slider);
	}

	int w = ((v - slider_getMarkerLength(slider))
		* (value  - slider_getScaleStart(slider))
		/ (slider_getScaleEnd(slider) - slider_getScaleStart(slider)));

	clamp(w, 0, slider_getMarkerLength(slider));

	return w;
}

void slider_mousePressed(void* widget, mouseEvent_t* mouseEvent)
{
	guiSlider_t *slider = (guiSlider_t*)widget;
	int val;
	if (mouseEvent->button == ME_LEFT
		&& mouseEvent->pos.x >= 0 && mouseEvent->pos.x <= widget_getWidth(slider)
		&& mouseEvent->pos.y >= 0 && mouseEvent->pos.y <= widget_getHeight(slider)) {
			if (slider_getOrientation(slider) == SLIDER_HORIZONTAL) {
				val = slider_markerPositionToValue(slider, mouseEvent->pos.x - slider_getMarkerLength(slider) / 2);
			} else {
				val = slider_markerPositionToValue(slider, widget_getHeight(slider) - mouseEvent->pos.y - slider_getMarkerLength(slider) / 2);
			}
			slider_setValue(slider, val);
			// FIXME event
		}
}

