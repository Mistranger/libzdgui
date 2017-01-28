#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

// Debug levels
#define DEBUG_NONE 0x00
#define DEBUG_ERRORS 0x01
#define DEBUG_WARNINGS 0x02
#define DEBUG_INFO 0x04
#define DEBUG_DEBUG 0x08
#define DEBUG_ALL 0x0F

// Global debug level
#define DEBUGLEVEL DEBUG_NONE //(DEBUG_INFO + DEBUG_ERRORS + DEBUG_WARNINGS)

#define _C_ ,

#define PrintFunction() \
	{ ACS_PrintLogStr(s"\Chfunction \Cn%s \Chat line \Cn%d \Chin file \Cn%s:", __func__, __LINE__, __FILE__); }

#if DEBUGLEVEL > DEBUG_NONE
#define guiAssert(cond)   \
	do { if (!(cond)) {   \
			PrintFunction();  \
			abort(); }} while (0)
#else
#define guiAssert(args) {}
#endif

#if (DEBUGLEVEL & DEBUG_ERRORS)
#define guiError(args) \
	do { PrintFunction(); ACS_PrintLogStr(s"    \CgERROR: " args); } while (0)
#else
#define guiError(args)
#endif

#if (DEBUGLEVEL & DEBUG_WARNINGS)
#define guiWarning(args) \
	do { PrintFunction(); ACS_PrintLogStr(s"    \CdWARNING\C-: " args); } while (0)
#else
#define guiWarning(args)
#endif

#if (DEBUGLEVEL & DEBUG_INFO)
#define guiInfo(args) \
	do { PrintFunction(); ACS_PrintLogStr(s"    \CtINFO\C-: " args); } while (0)
#else
#define guiInfo(args)
#endif

#if (DEBUGLEVEL & DEBUG_DEBUG)
#define guiDebugPrint(args) \
	do { PrintFunction(); ACS_PrintLogStr(s"    \CeDEBUG\C-: " args); } while (0)
#else
#define guiDebugPrint(args)
#endif

#endif // DEBUG_H_INCLUDED
