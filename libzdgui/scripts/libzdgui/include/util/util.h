#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

//extern bool EnableDebugPrint;
//extern bool EnableAssert;

#define Assert(cond) \
	do { if (!(cond)) { \
		abort(); }} while (0)

#define clamp(val, min, max) \
	(val < min ? (val = min) : (val > max ? (val = max) : (val)))


			
#endif // UTIL_H_INCLUDED
