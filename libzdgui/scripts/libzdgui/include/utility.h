#ifndef _I_UTILITY_
#define _I_UTILITY_

#ifdef DEBUG
	#define DebugPrint(funcname, message) (Log(s:"\crDEBUG: \ci", s:funcName, s:": \cq", s:message);)
#else
	#define DebugPrint(funcname, message) ()
#endif

#endif