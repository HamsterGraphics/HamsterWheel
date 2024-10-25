/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "IOperatingSystem.h"
#include "Base/Assert.h"
#include "Base/NameOf.h"
#include "Base/TypeTraits.h"

///////////////////////////////////////////////////////////////////////////////////
// Console Log
///////////////////////////////////////////////////////////////////////////////////
#define MAX_MESSAGE_BUFFER_LENGTH 4096
#define MAX_LOG_PREFIX_BUFFER_LENGTH 16

static HANDLE g_consoleHandle;
static Mutex g_logMutex;
static char g_messageBuffer[MAX_MESSAGE_BUFFER_LENGTH];

typedef struct LogLevelStyle
{
	char Prefix[MAX_LOG_PREFIX_BUFFER_LENGTH];
	int32 PrefixLength;
	WORD Attribute;
} LogLevelStyle;
static LogLevelStyle LogStyles[hg::EnumCount<LogLevel>()];

bool Log_Init(ConsoleInfo* pInfo)
{
	Mutex_Create(&g_logMutex);
	g_consoleHandle = (HANDLE)pInfo->OutputHandle;
	Assert(g_consoleHandle != NULL);

	{
		auto& logStyle = LogStyles[LOG_LEVEL_INFO];
		constexpr char prefix[] = "[INFO] ";
		strcpy_s(logStyle.Prefix, MAX_LOG_PREFIX_BUFFER_LENGTH, prefix);
		logStyle.PrefixLength = COUNTOF(prefix);
		logStyle.Attribute = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}

	{
		auto& logStyle = LogStyles[LOG_LEVEL_TRACE];
		constexpr char prefix[] = "[TRACE] ";
		strcpy_s(logStyle.Prefix, MAX_LOG_PREFIX_BUFFER_LENGTH, prefix);
		logStyle.PrefixLength = COUNTOF(prefix);
		logStyle.Attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}

	{
		auto& logStyle = LogStyles[LOG_LEVEL_WARNING];
		constexpr char prefix[] = "[WARN] ";
		strcpy_s(logStyle.Prefix, MAX_LOG_PREFIX_BUFFER_LENGTH, prefix);
		logStyle.PrefixLength = COUNTOF(prefix);
		logStyle.Attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}

	{
		auto& logStyle = LogStyles[LOG_LEVEL_ERROR];
		constexpr char prefix[] = "[ERROR] ";
		strcpy_s(logStyle.Prefix, MAX_LOG_PREFIX_BUFFER_LENGTH, prefix);
		logStyle.PrefixLength = COUNTOF(prefix);
		logStyle.Attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
	}

	{
		auto& logStyle = LogStyles[LOG_LEVEL_FATAL];
		constexpr char prefix[] = "[FATAL] ";
		strcpy_s(logStyle.Prefix, MAX_LOG_PREFIX_BUFFER_LENGTH, prefix);
		logStyle.PrefixLength = COUNTOF(prefix);
		logStyle.Attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
	}

	return true;
}

void Log_Shutdown()
{
	Mutex_Destroy(&g_logMutex);
}

void Log_PrintFormat(LogLevel level, const char* format, ...)
{
	ScopedMutexLock mutexLock(g_logMutex);

	const auto& logStyle = LogStyles[level];
	strcpy_s(g_messageBuffer, logStyle.PrefixLength, logStyle.Prefix);
	::SetConsoleTextAttribute(g_consoleHandle, logStyle.Attribute);

	va_list args;
	va_start(args, format);
	int length = vsnprintf(g_messageBuffer + logStyle.PrefixLength - 1, COUNTOF(g_messageBuffer) - logStyle.PrefixLength, format, args);
	va_end(args);

	printf("%s\n", g_messageBuffer);
	::OutputDebugStringA(g_messageBuffer);

	::SetConsoleTextAttribute(g_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	if (LOG_LEVEL_FATAL == level)
	{
		HG_BREAKPOINT;
	}
}