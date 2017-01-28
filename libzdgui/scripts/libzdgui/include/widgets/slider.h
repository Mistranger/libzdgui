#ifndef WIDGETS_SLIDER_H_INCLUDED
#define WIDGETS_SLIDER_H_INCLUDED

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/

#include "event.h"
#include "event/event_mouse.h"
#include "image.h"
#include "util/string.h"
#include "widget.h"

/*----------------------------------------------------------------------------
-- Defines
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-- Types
----------------------------------------------------------------------------*/

struct guiSlider;

extern const char *SliderType;

typedef struct guiSlider_vf {
	const char *(*w_typename)(struct guiSlider *widget);
	void (*w_destructor)(struct guiWidget *widget);
	struct guiRectangle *(*w_getChildrenArea)(const struct guiWidget *widget);
	struct guiWidget *(*w_getWidgetAt)(const struct guiWidget *widget, vec2i pos);
	void(*w_draw)(const struct guiSlider *slider, guiGraphics *graphics);
	void(*w_tick)(struct guiWidget *widget);
	bool(*w_isWidgetExisting)(struct guiWidget *widget, const struct guiWidget *exist);
	void(*w_setFocusHandler)(struct guiWidget *widget, void *focus);
} guiSlider_vf_t;

typedef enum guiSliderOrientation {
	SLIDER_HORIZONTAL = 0,
	SLIDER_VERTIAL,
} guiSliderOrientation_t;

typedef struct guiSlider {
	guiWidget widget;

	bool isPressed;
	bool hasMouse;
	int value;
	int start, end;
	int markerLength;
	int stepLength;
	guiSliderOrientation_t orientation;

	guiImage *markerImage;
	guiImage *backgroundImage;
	guiImage *disabledBackgroundImage;
} guiSlider;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

const char *slider_typename(guiSlider *widget);
guiSlider *slider_new(const int maxScale);

void slider_init(guiSlider *slider, const int maxScale);
void slider_draw(const guiSlider *slider, guiGraphics *graphics);

int slider_markerPositionToValue(const guiSlider *slider, int v);
int slider_valueToMarkerPosition(const guiSlider *slider, int value);

#define slider_getOrientation(_widget) (((guiSlider*)_widget)->orientation)
#define slider_setOrientation(_widget, _orient) { ((guiSlider*)_widget)->orientation = _orient; }
#define slider_getValue(_widget) (((guiSlider*)_widget)->value)
#define slider_setValue(_widget, _value)   \
	{ clamp((_value), slider_getScaleStart((guiSlider*)(_widget)), slider_getScaleEnd((guiSlider*)(_widget)));   \
		((guiSlider*)_widget)->value = (_value); }
#define slider_getMarkerLength(_widget) (((guiSlider*)_widget)->markerLength)
#define slider_setMarkerLength(_widget, _value) { ((guiSlider*)_widget)->markerLength = _value; }
#define slider_getStepLength(_widget) (((guiSlider*)_widget)->stepLength)
#define slider_setStepLength(_widget, _value) { ((guiSlider*)_widget)->stepLength = _value; }

#define slider_getScaleStart(_widget) (((guiSlider*)_widget)->start)
#define slider_getScaleEnd(_widget) (((guiSlider*)_widget)->end)

#define slider_setScale(_widget, _start, _end) { ((guiSlider*)_widget)->start = _start; ((guiSlider*)_widget)->_end = _end; }
#define slider_setScaleStart(_widget, _start) { ((guiSlider*)_widget)->start = _start; }
#define slider_setScaleEnd(_widget, _end) { ((guiSlider*)_widget)->start = _end; }

#define slider_getMarkerPosition(_widget) (slider_valueToMarkerPosition(((guiSlider*)_widget), ((guiSlider*)_widget)->value))

#define slider_getMarkerImage(_widget) (((guiSlider*)_widget)->markerImage)
#define slider_setMarkerImage(_widget, _image) { ((guiSlider*)_widget)->markerImage = _image; }
#define slider_getBackgroundImage(_widget) (((guiSlider*)_widget)->backgroundImage)
#define slider_setBackgroundImage(_widget, _image) { ((guiSlider*)_widget)->backgroundImage = _image; }
#define slider_getDisabledBackgroundImage(_widget) (((guiSlider*)_widget)->disabledBackgroundImage)
#define slider_setDisabledBackgroundImage(_widget, _image) { ((guiSlider*)_widget)->disabledBackgroundImage = _image; }

// Event listeners
void slider_mousePressed(void *widget, guiMouseEvent *mouseEvent);


#endif // WIDGETS_SLIDER_H_INCLUDED
