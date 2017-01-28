#include "libzdgui.h"
#include "unittest/CuTest.h"

void test_queue_create(CuTest *tc)
{
	queue_t *q = queue_init();
	CuAssertIntEquals(tc, 0, queue_size(q));
	queue_push(q, (void *)100);
	CuAssertIntEquals(tc, 1, queue_size(q));
	queue_push(q, (void *)200);
	queue_push(q, (void *)300);
	CuAssertIntEquals(tc, 3, queue_size(q));
	queue_pop(q);
	CuAssertIntEquals(tc, 2, queue_size(q));
	CuAssertIntEquals(tc, 200, (int)queue_front(q));
}
/*
void test_queue_mouse_event(CuTest *tc)
{
	queue_t *q = queue_init();
	vec2i_t pos = {123, 456};

	event_t *event = new(event_t);
	event->eventType = EV_Mouse;
	mouseEvent_t *lreleased = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
	lreleased->pos = pos;
	lreleased->type = ME_RELEASED;
	lreleased->button = LEFT;
	event->events.mouse = lreleased;
	queue_push(q, event);

	event_t *event = (event_t*)queue_front(q);
	CuAssertIntEquals(tc, EV_Mouse, event->eventType);
	CuAssertIntEquals(tc, ME_RELEASED, event->events.mouse->type);

	event_t *event2 = new(event_t);
	event2->eventType = EV_Mouse;
	lreleased = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
	lreleased->pos = pos;
	lreleased->type = ME_PRESSED;
	lreleased->button = LEFT;
	event2->events.mouse = lreleased;
	queue_push(q, event2);

	event_t *event3 = new(event_t);
	event3->eventType = EV_Mouse;
	lreleased = (mouseEvent_t*)malloc(sizeof(mouseEvent_t));
	lreleased->pos = pos;
	lreleased->type = ME_PRESSED;
	lreleased->button = RIGHT;
	event3->events.mouse = lreleased;
	queue_push(q, event3);

	while (queue_size(q) > 0) {
		event_t *event = (event_t*)queue_front(q);
		CuAssertIntEquals(tc, EV_Mouse, event->eventType);
		queue_pop(q);
	}

	CuAssertIntEquals(tc, 0, queue_size(q));
}
*/
CuSuite *test_util_queue_getSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_queue_create);
	//SUITE_ADD_TEST(suite, test_queue_mouse_event);
	return suite;
}
