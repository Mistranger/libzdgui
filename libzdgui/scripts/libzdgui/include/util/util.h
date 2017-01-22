#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

//extern bool EnableDebugPrint;
//extern bool EnableAssert;

#define clamp(val, min, max) \
	(val < min ? (val = min) : (val > max ? (val = max) : (val)))

#define MIN(val1, val2) \
	(val1 < val2 ? val1 : val2)

			
#endif // UTIL_H_INCLUDED
