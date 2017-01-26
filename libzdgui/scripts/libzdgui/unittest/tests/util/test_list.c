#include "libzdgui.h"
#include "unittest/CuTest.h"

// Test list_t and associated functions

void test_list_create(CuTest *tc)
{
	list_t *l = list_new();
	CuAssertIntEquals(tc, 0, (list_size(l)));
	list_delete(l);
}

void test_list_addremove(CuTest *tc)
{
	list_t *l = list_new();
	list_push_back(l, (void*)100);
	list_push_back(l, (void*)200);
	list_push_back(l, (void*)300);
	list_push_front(l, (void*)400);
	list_push_front(l, (void*)500);
	list_push_front(l, (void*)600);
	CuAssertIntEquals(tc, 6, (list_size(l)));
	CuAssertIntEquals(tc, 600, ((int)list_front(l)->data));
	CuAssertIntEquals(tc, 300, ((int)list_back(l)->data));
	list_pop_back(l);
	CuAssertIntEquals(tc, 200, ((int)list_back(l)->data));
	CuAssertIntEquals(tc, 5, (list_size(l)));
	list_delete(l);
}

void test_list_insert_erase(CuTest *tc)
{
	list_t *l = list_new();
	list_push_back(l, (void*)100);
	listNode_t *n = list_front(l);
	list_insert(l, n, true, (void*)200);
	list_insert(l, n, false, (void*)300);
	CuAssertIntEquals(tc, 200, ((int)list_front(l)->data));
	CuAssertIntEquals(tc, 300, ((int)list_back(l)->data));
	CuAssertIntEquals(tc, 3, (list_size(l)));
	list_erase(l, n);
	CuAssertIntEquals(tc, 2, (list_size(l)));
	CuAssertIntEquals(tc, 200, ((int)list_front(l)->data));
	CuAssertIntEquals(tc, 300, ((int)list_back(l)->data));
	list_erase(l, list_front(l));
	list_erase(l, list_back(l));
	CuAssertIntEquals(tc, 0, (list_size(l)));
}

CuSuite* test_util_list_getSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_list_create);
	SUITE_ADD_TEST(suite, test_list_addremove);
	SUITE_ADD_TEST(suite, test_list_insert_erase);
	return suite;
}

