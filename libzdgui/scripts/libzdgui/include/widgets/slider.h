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

struct guiSlider_s;

extern const char *SliderType;

typedef struct guiSlider_vf {
	const char* (*w_typename)(struct guiSlider_s *widget);
	void (*w_destructor)(struct guiWidget_s *widget);
	struct guiRectangle_s* (*w_getChildrenArea)(const struct guiWidget_s *widget);
	struct guiWidget_s* (*w_getWidgetAt)(const struct guiWidget_s *widget, vec2i_t pos);
	void(*w_draw)(const struct guiSlider_s *slider, guiGraphics_t *graphics);
	void(*w_tick)(struct guiWidget_s *widget);
	bool(*w_isWidgetExisting)(struct guiWidget_s *widget, const struct guiWidget_s *exist);
	void(*w_setFocusHandler)(struct guiWidget_s *widget, void *focus);
} guiSlider_vf_t;

typedef enum guiSliderOrientation {
	SLIDER_HORIZONTAL = 0,
	SLIDER_VERTIAL,
} guiSliderOrientation_t;

typedef struct guiSlider_s
{
	guiWidget_t widget;

	bool isPressed;
	bool hasMouse;
	int value;
	int start, end;
	int markerLength;
	int stepLength;
	guiSliderOrientation_t orientation;

	guiImage_t *markerImage;
	guiImage_t *backgroundImage;
	guiImage_t *disabledBackgroundImage;
} guiSlider_t;

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

const char* slider_typename(guiSlider_t *widget);
guiSlider_t* slider_new(const int maxScale);

void slider_init(guiSlider_t* slider, const int maxScale);
void slider_draw(const guiSlider_t* slider, guiGraphics_t* graphics);

int slider_markerPositionToValue(const guiSlider_t* slider, int v);
int slider_valueToMarkerPosition(const guiSlider_t* slider, int value);

#define slider_getOrientation(_widget) (((guiSlider_t*)_widget)->orientation)
#define slider_setOrientation(_widget, _orient) { ((guiSlider_t*)_widget)->orientation = _orient; }
#define slider_getValue(_widget) (((guiSlider_t*)_widget)->value)
#define slider_setValue(_widget, _value)   \
	{ clamp((_value), slider_getScaleStart((guiSlider_t*)(_widget)), slider_getScaleEnd((guiSlider_t*)(_widget)));   \
	((guiSlider_t*)_widget)->value = (_value); }
#define slider_getMarkerLength(_widget) (((guiSlider_t*)_widget)->markerLength)
#define slider_setMarkerLength(_widget, _value) { ((guiSlider_t*)_widget)->markerLength = _value; }
#define slider_getStepLength(_widget) (((guiSlider_t*)_widget)->stepLength)
#define slider_setStepLength(_widget, _value) { ((guiSlider_t*)_widget)->stepLength = _value; }

#define slider_getScaleStart(_widget) (((guiSlider_t*)_widget)->start)
#define slider_getScaleEnd(_widget) (((guiSlider_t*)_widget)->end)

#define slider_setScale(_widget, _start, _end) { ((guiSlider_t*)_widget)->start = _start; ((guiSlider_t*)_widget)->_end = _end; }
#define slider_setScaleStart(_widget, _start) { ((guiSlider_t*)_widget)->start = _start; }
#define slider_setScaleEnd(_widget, _end) { ((guiSlider_t*)_widget)->start = _end; }

#define slider_getMarkerPosition(_widget) (slider_valueToMarkerPosition(((guiSlider_t*)_widget), ((guiSlider_t*)_widget)->value))

#define slider_getMarkerImage(_widget) (((guiSlider_t*)_widget)->markerImage)
#define slider_setMarkerImage(_widget, _image) { ((guiSlider_t*)_widget)->markerImage = _image; }
#define slider_getBackgroundImage(_widget) (((guiSlider_t*)_widget)->backgroundImage)
#define slider_setBackgroundImage(_widget, _image) { ((guiSlider_t*)_widget)->backgroundImage = _image; }
#define slider_getDisabledBackgroundImage(_widget) (((guiSlider_t*)_widget)->disabledBackgroundImage)
#define slider_setDisabledBackgroundImage(_widget, _image) { ((guiSlider_t*)_widget)->disabledBackgroundImage = _image; }

// Event listeners
void slider_mousePressed(void *widget, mouseEvent_t *mouseEvent);


#endif // WIDGETS_SLIDER_H_INCLUDED
