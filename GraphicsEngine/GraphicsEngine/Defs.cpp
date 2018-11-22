#include "Defs.h"

#include <cstdarg>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <assert.h>

#ifdef STD_OUTPUT

void log(ushort color, const char* format, ...)
{
	static char tmp[4096];
	static char tmp2[4096];

	static va_list ap;
	va_start(ap, format);
	vsprintf_s(tmp, 4096, format, ap);
	va_end(ap);

	sprintf_s(tmp2, 4096, "[%s]: %s\n", GetLogTypeStr(color), tmp);

	OutputDebugString(tmp2);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	std::cout << tmp << std::endl;
}

const char* GetLogTypeStr(ushort color)
{
	if (LOG_INFO & color) return "INFO";
	if (LOG_WARN & color) return "WARN";
	if (LOG_ERROR & color) return "ERROR";
	if (LOG_CMD & color) return "CMD";

	return "error";
}

#else

void log(ushort type, const char* format, ...)
{
	static char tmp[4096];
	static char tmp2[4096];

	static va_list ap;
	va_start(ap, format);
	vsprintf_s(tmp, 4096, format, ap);
	va_end(ap);

	sprintf_s(tmp2, 4096, "[%s]: %s\n", GetLogTypeStr(type), tmp);

	OutputDebugString(tmp2);
}

const char* GetLogTypeStr(ushort type)
{
	static const char* types[] = {
		"INFO", "WARN", "ERROR", "CMD"
	};

	if (type >= 0 && type < LOG_NONE)
		return types[type];

	return "error";
}

#endif
