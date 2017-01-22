#include "libzdgui.h"
#include "unittest/CuTest.h"

void test_vector_create(CuTest *tc)
{
	vector_t *v = vector_new();
	CuAssertIntEquals(tc, VECTOR_CAPACITY, v->capacity);
}

void test_vector_add(CuTest *tc)
{
	vector_t *v = vector_new();
	for (int i = 0; i < VECTOR_CAPACITY + 2; ++i) {
		int *a = new(int);
		*a = i;
		vector_push_back(v, a);
	}
	
	
	CuAssertIntEquals(tc, vector_size(v), v->size);
	CuAssertIntEquals(tc, VECTOR_CAPACITY * 2, vector_capacity(v));
	CuAssertIntEquals(tc, VECTOR_CAPACITY + 2, v->size);
	CuAssertIntEquals(tc, 4, *((int*)v->data[4]));
	CuAssertIntEquals(tc, 4, vector_get(v, 4, int));
}

void test_vector_addremove(CuTest *tc)
{
	vector_t *v = vector_new();
	for (int i = 0; i < VECTOR_CAPACITY + 2; ++i) {
		int *a = new(int);
		*a = i;
		vector_push_back(v, a);
	}
	vector_pop_back(v);
	vector_pop_back(v);
	vector_pop_back(v);
	CuAssertIntEquals(tc, (VECTOR_CAPACITY - 1), (vector_size(v)));
	CuAssertIntEquals(tc, (VECTOR_CAPACITY * 2), (vector_capacity(v)));
	CuAssertIntEquals(tc, (NULL), vector_get(v, vector_capacity(v), int));
	CuAssertIntEquals(tc, (NULL), vector_get(v, vector_capacity(v) - 1, int));
	CuAssertIntEquals(tc, (2), vector_get(v, vector_size(v) - 1, int));
}

void test_vector_resize(CuTest *tc)
{
	vector_t *v = vector_new();
	for (int i = 1; i < VECTOR_CAPACITY + 2; ++i) {
		int *a = new(int);
		*a = i;
		vector_push_back(v, a);
	}
	vector_resize(v, 1);
	CuAssertIntEquals(tc, 1, (vector_size(v)));
	CuAssertIntEquals(tc, (NULL), vector_get(v, 1, int));
	CuAssertIntEquals(tc, 1, vector_get(v, 0, int));
	vector_resize(v, 10);
	CuAssertIntEquals(tc, 1, (vector_size(v)));
	CuAssertIntEquals(tc, 10, (vector_capacity(v)));
	CuAssertIntEquals(tc, (NULL), vector_get(v, 1, int));
}

void test_vector_inserterase(CuTest *tc)
{
	vector_t *v = vector_new();
	for (int i = 0; i < VECTOR_CAPACITY + 4; ++i) {
		int *a = new(int);
		*a = i;
		vector_push_back(v, a);
	}
	int *a = new(int);
	*a = 100;
	vector_insert(v, 1, a);
	CuAssertIntEquals(tc, 9, (vector_size(v)));
	CuAssertIntEquals(tc, VECTOR_CAPACITY * 4, (vector_capacity(v)));
	CuAssertIntEquals(tc, 1, vector_get(v, 2, int));
	CuAssertIntEquals(tc, 100, vector_get(v, 1, int));
	vector_erase(v, 0);
	CuAssertIntEquals(tc, 100, vector_get(v, 0, int));
	CuAssertIntEquals(tc, 8, (vector_size(v)));
}

CuSuite* test_util_vector_getSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_vector_create);
	SUITE_ADD_TEST(suite, test_vector_add);
	SUITE_ADD_TEST(suite, test_vector_addremove);
	SUITE_ADD_TEST(suite, test_vector_resize);
	SUITE_ADD_TEST(suite, test_vector_inserterase);
	return suite;
}