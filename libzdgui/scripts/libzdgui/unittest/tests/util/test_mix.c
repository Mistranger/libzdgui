#include "libzdgui.h"
#include "unittest/CuTest.h"

void test_vector_vectorOfStrings(CuTest *tc)
{
	vector_t *v = vector_new(sizeof(string_t*));
	
	string_t *str = string_new_char("first");
	string_t *str2 = string_new_char("second");
	string_t *str3 = string_new_char("third");
	string_t *str4 = string_new_char("fourth");
	string_t *str5 = string_new_char("fifth");
	
	vector_push_back(v, &str);
	vector_push_back(v, &str2);
	vector_push_back(v, &str3);
	vector_push_back(v, &str4);
	vector_push_back(v, &str5);
	
	CuAssertIntEquals(tc, 5, (vector_size(v)));
	CuAssertIntEquals(tc, VECTOR_CAPACITY * 2, (vector_capacity(v)));
	str = string_substr(str, 1, 1);
	vector_push_back(v, &str);
	string_t *s = vector_get(v, vector_size(v) - 1, string_t*);
	CuAssertPtrEquals(tc, str, s);
	CuAssertStrEquals(tc, "second", str2->s);
	CuAssertStrEquals(tc, "i", s->s);
	
}

CuSuite* test_util_mix_getSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, test_vector_vectorOfStrings);
	return suite;
}