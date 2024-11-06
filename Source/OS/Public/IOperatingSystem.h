/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "OSConfig.h"

 ///////////////////////////////////////////////////////////////////////////////////
 // CPU
 ///////////////////////////////////////////////////////////////////////////////////
#if defined(HG_CPU_ARCH_X86)
typedef X86Features ISAFeatures;
#elif defined(HG_CPU_ARCH_AARCH64)
typedef AARCH64Features ISAFeatures;
#endif

typedef struct CPUInfo
{
	char Vendor[128];
	char Brand[128];

	// Vendor
	bool IsAMD : 1;
	bool IsIntel : 1;

	// Processor
	int16 PhyscialProcessorCount;
	int16 LogicalProcessorCount;

	// Cache
	uint32 L1CacheSize;
	uint32 L2CacheSize;
	uint32 L3CacheSize;

	// Features
	ISAFeatures Features;
} CPUInfo;

C_ABI HG_OS_API bool HG_CDECL CPU_InitInfo(CPUInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// DRAM
///////////////////////////////////////////////////////////////////////////////////
typedef struct DRAMInfo
{
	uint64 TotalSize;
	uint64 AvailableSize;
} DRAMInfo;

C_ABI HG_OS_API bool HG_CDECL DRAM_InitInfo(DRAMInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// Power
///////////////////////////////////////////////////////////////////////////////////
typedef struct PowerInfo
{
	float BatteryLifePercent;

	bool IsACMode : 1;
	bool IsBatteryMode : 1;
	bool Padding : 6;
} PowerInfo;

C_ABI HG_OS_API bool HG_CDECL Power_UpdateStatus(PowerInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// Monitor
///////////////////////////////////////////////////////////////////////////////////
typedef struct MonitorInfo
{
#if defined(HG_PLATFORM_WINDOWS)
	WCHAR DisplayName[128];
	WCHAR AdapterName[128];
#endif

	Rect MonitorRect;
	Rect WorkRect;
	uint32 DPI[2];
	uint32 PhyscialWidth;
	uint32 PhyscialHeight;
	uint32 Index;
} MonitorInfo;

C_ABI HG_OS_API bool HG_CDECL Monitor_InitInfo(MonitorInfo* pInfo, uint32& monitorCount);

///////////////////////////////////////////////////////////////////////////////////
// Time
///////////////////////////////////////////////////////////////////////////////////
typedef struct TimeInfo
{
	int64 Frequency;
	float SecondsPerCycle;
} TimeInfo;

C_ABI HG_OS_API bool HG_CDECL Time_Init(TimeInfo* pInfo);
C_ABI HG_OS_API int64 HG_CDECL Time_QueryCounter();

///////////////////////////////////////////////////////////////////////////////////
// Console
///////////////////////////////////////////////////////////////////////////////////
typedef struct ConsoleInfo
{
	void* OutputHandle;
	void* WindowHandle;
} ConsoleInfo;

C_ABI HG_OS_API bool HG_CDECL Console_Init(ConsoleInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// Log
///////////////////////////////////////////////////////////////////////////////////
typedef enum LogLevel
{
	LOG_LEVEL_INFO,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL
} LogLevel;

C_ABI HG_OS_API bool HG_CDECL Log_Init(ConsoleInfo* pInfo);
C_ABI HG_OS_API void HG_CDECL Log_Shutdown();
C_ABI HG_OS_API void HG_CDECL Log_PrintFormat(LogLevel level, const char* format, ...);

#if 1
#define LOG_INFO(format, ...) Log_PrintFormat(LOG_LEVEL_INFO, format, __VA_ARGS__)
#define LOG_TRACE(format, ...) Log_PrintFormat(LOG_LEVEL_TRACE, format, __VA_ARGS__)
#define LOG_WARNING(format, ...) Log_PrintFormat(LOG_LEVEL_WARNING, format, __VA_ARGS__)
#define LOG_ERROR(format, ...) Log_PrintFormat(LOG_LEVEL_ERROR, format, __VA_ARGS__)
#define LOG_FATAL(format, ...) Log_PrintFormat(LOG_LEVEL_FATAL, format, __VA_ARGS__)
#else
#define LOG_INFO
#define LOG_TRACE
#define LOG_WARNING
#define LOG_ERROR
#define LOG_FATAL
#endif

///////////////////////////////////////////////////////////////////////////////////
// Environment Variable
///////////////////////////////////////////////////////////////////////////////////
C_ABI HG_OS_API bool HG_CDECL EnvironmentVariable_Contains(const char* key);
C_ABI HG_OS_API const char* HG_CDECL EnvironmentVariable_Get(const char* key);
C_ABI HG_OS_API bool HG_CDECL EnvironmentVariable_Set(const char* key, const char* value);

///////////////////////////////////////////////////////////////////////////////////
// Module
///////////////////////////////////////////////////////////////////////////////////
C_ABI HG_OS_API void* HG_CDECL Module_Load(const char* pFilePath);
C_ABI HG_OS_API void HG_CDECL Module_Unload(void* pModule);
C_ABI HG_OS_API void* HG_CDECL Module_GetFunctionAddress(void* pModule, const char* pFunctionName);

///////////////////////////////////////////////////////////////////////////////////
// Thread
///////////////////////////////////////////////////////////////////////////////////
typedef void (*ThreadFunction)(void*);
typedef struct ThreadInfo
{
	char ThreadName[MAX_THREAD_NAME_LENGTH];
	void* ThreadHandle;
	void* Data;
	ThreadFunction ThreadMain;
} ThreadInfo;

C_ABI HG_OS_API uint32 HG_CDECL Thread_GetCurrentID();
C_ABI HG_OS_API const char* HG_CDECL Thread_GetCurrentName();
C_ABI HG_OS_API void HG_CDECL Thread_SetCurrentName(const char* pName);
C_ABI HG_OS_API void HG_CDECL Thread_Sleep(uint32 seconds);
C_ABI HG_OS_API bool HG_CDECL Thread_Create(ThreadInfo* pInfo);
C_ABI HG_OS_API void HG_CDECL Thread_Join(ThreadInfo* pInfo);
C_ABI HG_OS_API void HG_CDECL Thread_Detach(ThreadInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// Mutex
///////////////////////////////////////////////////////////////////////////////////
typedef struct Mutex
{
#if defined(HG_PLATFORM_WINDOWS)
	CRITICAL_SECTION Handle;
#else
	pthread_mutex_t Handle;
#endif
	uint32 SpinCount = 2000;
} Mutex;

C_ABI HG_OS_API bool HG_CDECL Mutex_Init(Mutex* pMutex);
C_ABI HG_OS_API void HG_CDECL Mutex_Destroy(Mutex* pMutex);
C_ABI HG_OS_API void HG_CDECL Mutex_Acquire(Mutex* pMutex);
C_ABI HG_OS_API void HG_CDECL Mutex_Release(Mutex* pMutex);

#if defined(__cplusplus)

class ScopedLock
{
public:
	explicit ScopedLock(Mutex& mutex) : m_mutex(mutex)
	{
		Mutex_Acquire(&m_mutex);
	}

	ScopedLock(const ScopedLock&) = delete;
	ScopedLock& operator=(const ScopedLock&) = delete;

	~ScopedLock() noexcept
	{
		Mutex_Release(&m_mutex);
	}

private:
	Mutex& m_mutex;
};

#endif