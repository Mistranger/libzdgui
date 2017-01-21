#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

//extern bool EnableDebugPrint;
//extern bool EnableAssert;

#define clamp(val, min, max) \
	(val < min ? (val = min) : (val > max ? (val = max) : (val)))


			
#endif // UTIL_H_INCLUDED
