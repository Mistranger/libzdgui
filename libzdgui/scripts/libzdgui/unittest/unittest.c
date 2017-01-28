#include "libzdgui.h"
#include "unittest/CuTest.h"

CuSuite *test_util_queue_getSuite();
CuSuite *test_util_string_getSuite();
CuSuite *test_util_vector_getSuite();
CuSuite *test_util_mix_getSuite();
CuSuite *test_util_list_getSuite();

void RunAllTests()
{
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();
	CuSuiteAddSuite(suite, test_util_queue_getSuite());
	CuSuiteAddSuite(suite, test_util_string_getSuite());
	CuSuiteAddSuite(suite, test_util_vector_getSuite());
	CuSuiteAddSuite(suite, test_util_mix_getSuite());
	CuSuiteAddSuite(suite, test_util_list_getSuite());
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

#if 0
[[call("ScriptS"), script("Enter")]]
void test_main(void)
{
	RunAllTests();
}
#endif