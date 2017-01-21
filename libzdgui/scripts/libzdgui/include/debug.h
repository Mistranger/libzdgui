#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

// Debug levels
#define DEBUG_NONE 0
#define DEBUG_ERRORS 1
#define DEBUG_WARNINGS 2
#define DEBUG_INFO 3
#define DEBUG_DEBUG 4
#define DEBUG_ALL 5

// Global debug level
#define DEBUGLEVEL DEBUG_NONE

#define _C_ ,

#define PrintFunction() \
	{ ACS_PrintLogStr(s"\Chfunction \Cn%s \Chat line \Cn%d \Chin file \Cn%s:", __func__, __LINE__, __FILE__); }

#if DEBUGLEVEL > DEBUG_NONE
#define guiAssert(cond) \
	do { if (!(cond)) { \
		abort(); }} while (0)
#else
	#define guiAssert(args) do {} while (0)
#endif

#if DEBUGLEVEL == DEBUG_ERRORS || DEBUGLEVEL == DEBUG_ALL
	#define guiError(args) \
		do { PrintFunction(); ACS_PrintLogStr(s"		\CgERROR: " args); } while (0) 
#else
	#define guiError(args) do {} while (0)
#endif

#if DEBUGLEVEL == DEBUG_WARNINGS || DEBUGLEVEL == DEBUG_ALL
	#define guiWarning(args) \
		do { PrintFunction(); ACS_PrintLogStr(s"		\CdWARNING\C-: " args); } while (0) 
#else
	#define guiWarning(args) do {} while (0)
#endif

#if DEBUGLEVEL == DEBUG_INFO || DEBUGLEVEL == DEBUG_ALL
	#define guiInfo(args) \
		do { PrintFunction(); ACS_PrintLogStr(s"		\CINFO\C-: " args); } while (0) 
#else
	#define guiInfo(args) do {} while (0)
#endif

#if DEBUGLEVEL == DEBUG_DEBUG || DEBUGLEVEL == DEBUG_ALL
	#define guiDebugPrint(args) \
		do { PrintFunction(); ACS_PrintLogStr(s"		\CeDEBUG\C-: " args); } while (0) 
#else
	#define guiDebugPrint(args) do {} while (0)
#endif

#endif // DEBUG_H_INCLUDED
