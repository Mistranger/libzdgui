#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

//extern bool EnableDebugPrint;
//extern bool EnableAssert;

#define clamp(_val, _min, _max) \
	((_val) < (_min) ? ((_val) = (_min)) : ((_val) > (_max) ? ((_val) = (_max)) : ((_val))))

#define MIN(val1, val2) \
	(val1 < val2 ? val1 : val2)

#define MAX(val1, val2) \
	(val1 > val2 ? val1 : val2)


#endif // UTIL_H_INCLUDED
