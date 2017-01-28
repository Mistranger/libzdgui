#include "libzdgui.h"
#include "unittest/CuTest.h"

void test_vector_create(CuTest *tc)
{
	vector_t *v = vector_new(sizeof(int));
	CuAssertIntEquals(tc, VECTOR_CAPACITY, v->capacity);
}

void test_vector_add(CuTest *tc)
{
	vector_t *v = vector_new(sizeof(int));
	for (int i = 0; i < VECTOR_CAPACITY + 2; ++i) {
		vector_push_back(v, &i);
	}

	CuAssertIntEquals(tc, vector_size(v), v->size);
	CuAssertIntEquals(tc, VECTOR_CAPACITY * 2, vector_capacity(v));
	CuAssertIntEquals(tc, VECTOR_CAPACITY + 2, v->size);
	CuAssertIntEquals(tc, 4, vector_get(v, 4, int));
}

void test_vector_addremove(CuTest *tc)
{
	vector_t *v = vector_new(sizeof(int));
	for (int i = 0; i < VECTOR_CAPACITY + 2; ++i) {
		vector_push_back(v, &i);
	}
	vector_pop_back(v);
	vector_pop_back(v);
	vector_pop_back(v);
	CuAssertIntEquals(tc, (VECTOR_CAPACITY - 1), (vector_size(v)));
	CuAssertIntEquals(tc, (VECTOR_CAPACITY * 2), (vector_capacity(v)));
	//CuAssertIntEquals(tc, (NULL), vector_get(v, vector_capacity(v), int));
	CuAssertIntEquals(tc, 0, vector_get(v, 0, int));
	//CuAssertIntEquals(tc, (NULL), vector_get(v, vector_capacity(v) - 1, int));
	CuAssertIntEquals(tc, 1, vector_get(v, 1, int));
	CuAssertIntEquals(tc, (2), vector_get(v, vector_size(v) - 1, int));
	vector_pop_back(v);
	vector_pop_back(v);
	vector_pop_back(v);
	CuAssertIntEquals(tc, 0, (vector_size(v)));
}

void test_vector_resize(CuTest *tc)
{
	vector_t *v = vector_new(sizeof(int));
	for (int i = 1; i < VECTOR_CAPACITY + 2; ++i) {
		int *a = new (int);
		*a = i;
		vector_push_back(v, a);
	}
	vector_resize(v, 1);
	CuAssertIntEquals(tc, 1, (vector_size(v)));
	//CuAssertIntEquals(tc, (NULL), vector_get(v, 1, int));
	CuAssertIntEquals(tc, 1, vector_get(v, 0, int));
	vector_resize(v, 10);
	CuAssertIntEquals(tc, 1, (vector_size(v)));
	CuAssertIntEquals(tc, 10, (vector_capacity(v)));
	//CuAssertIntEquals(tc, (NULL), vector_get(v, 1, int));
}

void test_vector_inserterase(CuTest *tc)
{
	vector_t *v = vector_new(sizeof(int));
	for (int i = 0; i < VECTOR_CAPACITY + 4; ++i) {
		vector_push_back(v, &i);
	}
	int a = 100;
	vector_insert(v, 1, &a);
	CuAssertIntEquals(tc, 9, (vector_size(v)));
	CuAssertIntEquals(tc, VECTOR_CAPACITY * 4, (vector_capacity(v)));
	CuAssertIntEquals(tc, 1, vector_get(v, 2, int));
	//CuAssertIntEquals(tc, 100, vector_get(v, 1, int));
	vector_erase(v, 0);
	CuAssertIntEquals(tc, 100, vector_get(v, 0, int));
	CuAssertIntEquals(tc, 8, (vector_size(v)));
}

void test_vector_insertwidgets(CuTest *tc)
{
	guiGUI_t *gui = new (guiGUI_t);
	gui_init(gui, 640, 480);
	gui_setTop(gui, NULL);

	string_t *str = string_new_char("Hello world");
	vector_t *v = vector_new(sizeof(guiWidget_t *));
	CuAssertIntEquals(tc, sizeof(guiWidget_t *), v->elemSize);
	string_assign_char(str, "shithappens");
	guiButton *button = button_new(gui, str);
	string_assign_char(str, "shithappens2");
	guiButton *button2 = button_new(gui, str);
	string_assign_char(str, "shithappens");
	guiButton *button3 = button_new(gui, str);

	vector_push_back(v, &button);
	vector_push_back(v, &button2);
	vector_push_back(v, &button3);
	guiButton *w =  vector_get(v, 0, guiButton*);
	guiButton *w2 =  vector_get(v, 1, guiButton*);
	guiButton *w3 =  vector_get(v, 2, guiButton*);
	
	CuAssertPtrEquals(tc, button, w);
	CuAssertPtrEquals(tc, button2, w2);
	CuAssertPtrEquals(tc, button3, w3);
	CuAssertStrEquals(tc, "shithappens", button_getCaption(button)->s);

}

CuSuite *test_util_vector_getSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_vector_create);
	SUITE_ADD_TEST(suite, test_vector_add);
	SUITE_ADD_TEST(suite, test_vector_addremove);
	SUITE_ADD_TEST(suite, test_vector_resize);
	SUITE_ADD_TEST(suite, test_vector_inserterase);
	SUITE_ADD_TEST(suite, test_vector_insertwidgets);
	return suite;
}
