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

#define LIBZDGUI_VERSION_MAJOR 0
#define LIBZDGUI_VERSION_MINOR 0
#define LIBZDGUI_VERSION_PATCH 1

#define _stringify_(s) #s
#define _stringify(s) _stringify_(s)
#define libzdgui_getVersion() (_stringify(LIBZDGUI_VERSION_MAJOR) "." _stringify(LIBZDGUI_VERSION_MINOR) "." _stringify(LIBZDGUI_VERSION_PATCH))

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

int ACS_HudMessage(int flags, int id, int color, fixed x, fixed y, fixed holdTime, __str format, ...);
int ACS_HudMessageColor(int flags, int id, __str color, fixed x, fixed y, fixed holdTime, __str format, ...);
int ACS_HudMessageExt(int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, fixed oarg3, __str format, ...);
int ACS_HudMessageColorExt(int flags, int id, __str color, fixed x, fixed y, fixed holdTime, fixed alpha, __str format, ...);
int ACS_PrintLog(const char *format, ...);
int ACS_PrintLogStr(__str format, ...);

#define CharToStr(_cstr) \
  ( \
   ACS_BeginStrParam(), \
   __nprintf(_cstr), \
   ACS_EndStrParam() \
  )

#endif // SYSTEM_H_INCLUDED
