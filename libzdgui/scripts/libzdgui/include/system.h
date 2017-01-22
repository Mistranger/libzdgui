#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

// ZDoom fixed type by default
#pragma GDCC FIXED_LITERAL ON

/*----------------------------------------------------------------------------
--  Includes
----------------------------------------------------------------------------*/
#include <ACS_ZDoom.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdfix.h>
#include <string.h>
#include <limits.h>

/*----------------------------------------------------------------------------
--  Defines
----------------------------------------------------------------------------*/

// Type definitions

//typedef __str __str;

// ZDoom script types

#define EXT [[extern("ACS")]]
#define SCRIPT(n) [[address(n), call("ScriptI")]]
#define N_SCRIPT [[call("ScriptS")]]

#define OPEN [[script("Open")]]
#define ENTER [[script("Enter")]]
#define RETURN [[script("Return")]]
#define RESPAWN [[script("Respawn")]]
#define DEATH [[script("Death")]]
#define LIGHTNING [[script("Lightning")]]
#define UNLOADING [[script("Unloading")]]
#define DISCONNECT [[script("Disconnect")]]
#define INPUT [[script("Input")]] // GLOOME

#define NET [[script("Net")]]
#define CLIENTSIDE [[script("Clientside")]]
#define EVENT [[script("Event")]]


#include "debug.h"

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

#define new(type) ((type*)malloc(sizeof(type)));

__str ACS_StrParam (const char *format, ...);
int ACS_HudMessage(int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, fixed oarg3, __str format, ...);
int ACS_PrintLog(const char *format, ...);
int ACS_PrintLogStr(__str format, ...);

#define CharToStr(cstr) \
  ( \
   ACS_BeginStrParam(), \
   __nprintf(cstr), \
   ACS_EndStrParam() \
  )


  


#endif // SYSTEM_H_INCLUDED
