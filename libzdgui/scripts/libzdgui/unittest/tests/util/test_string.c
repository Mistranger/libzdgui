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
	CuAssertIntEquals(tc, 11, string_size(str3));
	CuAssertIntEquals(tc, str->len, 0);
	CuAssertIntEquals(tc, str->capacity, STRING_LENGTH);
	string_assign_char(str3, "Shit");
	CuAssertStrEquals(tc, "Shit", (char*)str3->s);
	CuAssertIntEquals(tc, 4, string_size(str3));
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

void test_string_copy(CuTest *tc)
{
	string_t *str = string_new_char("Hello world!");
	string_t *str2 = string_new();
	string_t *str3 = string_new();
	CuAssertIntEquals(tc, 0, string_size(str3));
	CuAssertIntEquals(tc, 12, string_size(str));
	string_copy(str2, str, 0, string_size(str));
	string_copy(str3, str, 6, string_size(str));
	CuAssertStrEquals(tc, "Hello world!", (char*)str2->s);
	CuAssertStrEquals(tc, "world!", (char*)str3->s);
	
}

void test_string_find(CuTest *tc)
{
	string_t *str = string_new_char("Please, replace the vowels in this sentence by asterisks.");
	size_t found = string_find_first_of_char(str, "aeiou", 0);
	while (found != STRING_NPOS) {
		str->s[found] = '*';
		found = string_find_first_of_char(str, "aeiou", found + 1);
	}
	CuAssertStrEquals(tc, "Pl**s*, r*pl*c* th* v*w*ls *n th*s s*nt*nc* by *st*r*sks.", (char*)str->s);
	
	string_t *str2 = string_new_char("There are two needles in this haystack with needles.");
	size_t c = string_find_c(str2, '.', 1);
	CuAssertIntEquals(tc, 51, c);
}

void test_string_substr(CuTest *tc)
{
	string_t *str = string_new_char("We think in generalities, but we live in details.");
	string_t *str2 = string_substr(str, 3, 5);
	CuAssertStrEquals(tc, "think", (char*)str2->s);
	string_t *str3 = string_substr(str, 12, STRING_NPOS);
	CuAssertStrEquals(tc, "generalities, but we live in details.", (char*)str3->s);
}

CuSuite* test_util_string_getSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_string_create);
	SUITE_ADD_TEST(suite, test_string_append);
	SUITE_ADD_TEST(suite, test_string_insert);
	SUITE_ADD_TEST(suite, test_string_erase);
	SUITE_ADD_TEST(suite, test_string_copy);
	SUITE_ADD_TEST(suite, test_string_find);
	SUITE_ADD_TEST(suite, test_string_substr);
	return suite;
}
