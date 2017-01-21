#include "system.h"

#include "widgets/label.h"

guiLabel_vf_t guiLabel_vtable = {
	widget_getChildrenArea,
	widget_getWidgetAt,
	label_draw,
	widget_tick,
	widget_isWidgetExisting
};

guiLabel_t* label_new(const char* caption)
{
	guiLabel_t *label = (guiLabel_t*)malloc(sizeof(guiLabel_t));
	label_init(label, caption);
	return label;
}

void label_init(guiLabel_t *label, const char *caption)
{
	widget_init((guiWidget_t*)label);
	label->widget.v = (guiWidget_vf_t*)&guiLabel_vtable;
	
	label_setCaption(label, caption);
}

const char* label_getCaption(const guiLabel_t* label)
{
	return label->caption;
}

void label_setCaption(guiLabel_t* label, const char* caption)
{
	strncpy(label->caption, caption, MAX_CAPTION_LENGTH);
}

void label_draw(const guiLabel_t* widget, guiGraphics_t* graphics)
{
	
}
