#include "libzdgui.h"
#include "unittest/CuTest.h"

void test_string_create(CuTest *tc)
{
	string_t *str = string_new();
	string_t *str2 = string_new_char("Test string");
	string_t *str3 = string_new_string(str2);
	CuAssertStrEquals(tc, "Test string", (char*)str2->s);
	CuAssertStrEquals(tc, "Test string", (char*)str3->s);
	CuAssertStrEquals(tc, "Test string", (char*)str3->s);
	CuAssertIntEquals(tc, str->len, 0);
	CuAssertIntEquals(tc, str->capacity, STRING_LENGTH);
}

void test_string_append(CuTest *tc)
{
	string_t *str = string_new_char("Hello ");
	string_t *str2 = string_new_char("world!");
	string_append_string(str, str2);
	CuAssertStrEquals(tc, "Hello world!", (char*)str->s);
	CuAssertIntEquals(tc, strlen("Hello world!"), string_size(str));
}

void test_string_insert(CuTest *tc)
{
	string_t *str = string_new_char("Hello world!");
	string_t *str2 = string_new_char("again, ");
	string_insert_string(str, 6, str2);
	CuAssertStrEquals(tc, "Hello again, world!", (char*)str->s);
	CuAssertIntEquals(tc, strlen("Hello again, world!"), string_size(str));
}

void test_string_erase(CuTest *tc)
{
	string_t *str = string_new_char("Hello world!");
	string_t *str2 = string_new_string(str);
	string_t *str3 = string_new_string(str);
	string_erase(str, 4, 1);
	string_erase(str2, 0, string_size(str2));
	string_erase(str3, 4, 666);
	CuAssertStrEquals(tc, "Hell world!", (char*)str->s);
	CuAssertStrEquals(tc, "", (char*)str2->s);
	CuAssertStrEquals(tc, "Hell", (char*)str3->s);
}

CuSuite* test_util_string_getSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_string_create);
	SUITE_ADD_TEST(suite, test_string_append);
	SUITE_ADD_TEST(suite, test_string_insert);
	SUITE_ADD_TEST(suite, test_string_erase);
	return suite;
}
