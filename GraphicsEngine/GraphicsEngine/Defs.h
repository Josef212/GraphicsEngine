#ifndef  __DEFS_H__
#define __DEFS_H__

// ------------------------------------------------------------------------------------

#include <Windows.h>

// ------------------------------------------------------------------------------------

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

// ------------------------------------------------------------------------------------

#define STD_OUTPUT

// ------------------------------------------------------------------------------------

#define LOG(color_or_type, format, ...) log(color_or_type, format, __VA_ARGS__);

#ifdef STD_OUTPUT

#define CONSOLE_TEXT_RED FOREGROUND_RED
#define CONSOLE_TEXT_GREEN FOREGROUND_GREEN
#define CONSOLE_TEXT_BLUE FOREGROUND_BLUE
#define CONSOLE_TEXT_YELLOW (CONSOLE_TEXT_RED | CONSOLE_TEXT_GREEN)
#define CONSOLE_TEXT_CIAN (CONSOLE_TEXT_GREEN | CONSOLE_TEXT_BLUE)
#define CONSOLE_TEXT_MAGENTA (CONSOLE_TEXT_RED |CONSOLE_TEXT_BLUE)
#define CONSOLE_TEXT_WHITE (CONSOLE_TEXT_RED | CONSOLE_TEXT_GREEN | CONSOLE_TEXT_BLUE)

#define LOG_INFO CONSOLE_TEXT_GREEN // Green already in white
#define LOG_WARN CONSOLE_TEXT_YELLOW
#define LOG_ERROR CONSOLE_TEXT_RED // Red already in magenta
#define LOG_CMD FOREGROUND_BLUE // Blue already in cian

#define PAUSE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_TEXT_WHITE); system("PAUSE");


const char* GetLogTypeStr(ushort color);

void log(ushort color, const char* format, ...);

#else

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_CMD 3
#define LOG_NONE 4

#define CONSOLE_TEXT_GREEN LOG_INFO
#define CONSOLE_TEXT_WHITE LOG_INFO
#define CONSOLE_TEXT_YELLOW LOG_WARN
#define CONSOLE_TEXT_RED LOG_ERROR
#define CONSOLE_TEXT_MAGENTA LOG_ERROR
#define CONSOLE_TEXT_BLUE LOG_CMD
#define CONSOLE_TEXT_CIAN LOG_CMD

#define PAUSE system("PAUSE");

void log(ushort type, const char* format, ...);
const char* GetLogTypeStr(ushort type);

#endif

// ------------------------------------------------------------------------------------

#define RELEASE( x )\
	{\
		if(x != nullptr)\
		{\
			delete x;\
			x = nullptr;\
		}\
	}

#define RELEASE_ARRAY( x )\
	{\
		if(x != nullptr)\
		{\
			delete[] x;\
			x = nullptr;\
		}\
	}

// ------------------------------------------------------------------------------------
// Config vars

#define DEFAULT_WIN_WIDTH 1280
#define DEFAULT_WIN_HEIGHT 720

// ------------------------------------------------------------------------------------
// Usefull macros

#define CLAMP(value, min, max) (value > max ? max : (value < min ? min : value))
#define CLAMP01(x) (CLAMP(x, 0.f, 1.f))
#define CLAMP_ROT(x) (CLAMP(x, 0.f, 360.f))
#define CLAMP_ROT_ROUND(x) (x > 360.f ? 0.f : (x < 0.f ? 360.f : x))

#define MAX (value, value2) value > value2 ? value : value2
#define MIN (value, value2) value < value2 ? value : value2

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define PI 3.14159265358979323846264338327950288

#define LOG_CREATION(name) LOG(LOG_INFO, "\t%s: Creation", name);
#define LOG_INIT(name) LOG(LOG_INFO, "\t%s: Init", name);
#define LOG_START(name) LOG(LOG_INFO, "\t%s: Start", name);
#define LOG_CLEANUP(name) LOG(LOG_INFO, "\t%s: CleanUp", name);
#define LOG_DESTRUCTION(name) LOG(LOG_INFO, "\t%s: Destruction", name);

// ------------------------------------------------------------------------------------
// Global enums

enum UpdateReturn
{
	UPDT_CONTINUE = 0,
	UPDT_STOP = 1,
	UPDT_ERROR = 2
};

// ------------------------------------------------------------------------------------



// ------------------------------------------------------------------------------------


#endif