#include "system.h"

int ACS_HudMessage(int flags, int id, int color, fixed x, fixed y, fixed holdTime, __str format, ...)
{
	int ret = 0;
	va_list arg;

	ACS_BeginHudMessage();

	va_start(arg, format);
	ret = __vnprintf_str(format, arg);
	va_end(arg);

	ACS_MoreHudMessage();
	ACS_OptHudMessage(flags, id, color, x, y, holdTime);
	ACS_EndHudMessage();

	return ret;
}

int ACS_HudMessageColor(int flags, int id, __str color, fixed x, fixed y, fixed holdTime, __str format, ...)
{
	int ret = 0;
	va_list arg;

	ACS_BeginHudMessage();

	va_start(arg, format);
	ret = __vnprintf_str(format, arg);
	va_end(arg);

	ACS_MoreHudMessage();
	ACS_OptHudMessageS(flags | HUDMSG_COLORSTRING, id, color, x, y, holdTime);
	ACS_EndHudMessage();

	return ret;
}

int ACS_HudMessageExt(int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, fixed oarg3, __str format, ...)
{
	int ret = 0;
	va_list arg;

	ACS_BeginHudMessage();

	va_start(arg, format);
	ret = __vnprintf_str(format, arg);
	va_end(arg);

	ACS_MoreHudMessage();
	ACS_OptHudMessage(flags, id, color, x, y, holdTime);
	ACS_EndHudMessageXXX(oarg1, oarg2, oarg3);

	return ret;
}

int ACS_HudMessageColorExt(int flags, int id, __str color, fixed x, fixed y, fixed holdTime, fixed alpha, __str format, ...)
{
	int ret = 0;
	va_list arg;

	ACS_BeginHudMessage();

	va_start(arg, format);
	ret = __vnprintf_str(format, arg);
	va_end(arg);

	ACS_MoreHudMessage();
	ACS_OptHudMessageS(flags | HUDMSG_COLORSTRING | HUDMSG_ALPHA, id, color, x, y, holdTime);
	ACS_EndHudMessageX(alpha);

	return ret;
}

int ACS_Print(const char *format, ...)
{
	int ret;
	va_list arg;

	ACS_BeginPrint();

	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);

	ACS_EndPrint();
	return ret;
}

int ACS_PrintLog(const char *format, ...)
{
	int ret;
	va_list arg;

	ACS_BeginLog();

	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);

	ACS_EndLog();
	return ret;
}

int ACS_PrintLogStr(__str format, ...)
{
	int ret;
	va_list arg;

	ACS_BeginLog();

	va_start(arg, format);
	ret = __vnprintf_str(format, arg);
	va_end(arg);

	ACS_EndLog();
	return ret;
}