/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Config.h"

#ifdef _WIN32
#include <windows.h>
#endif

 ///////////////////////////////////////////////////////////////////////////////////
 // CPU
 ///////////////////////////////////////////////////////////////////////////////////
typedef struct CPUInfo
{
	char Vendor[128];
	char Brand[128];

	// Processor
	int16 PhyscialProcessorCount;
	int16 LogicalProcessorCount;

	// Cache
	uint32 L1CacheSize;
	uint32 L2CacheSize;
	uint32 L3CacheSize;

	// Vendor
	bool IsAMD : 1;
	bool IsIntel : 1;

	// AVX
	bool HasAVX : 1;
	bool HasAVX2 : 1;
	bool HasAVX512CD : 1;
	bool HasAVX512ER : 1;
	bool HasAVX512F : 1;
	bool HasAVX512PF : 1;

	// SSE
	bool HasSSE : 1;
	bool HasSSE2 : 1;
	bool HasSSE3 : 1;
	bool HasSSE41 : 1;
	bool HasSSE42 : 1;
	bool HasSSSE3 : 1;

	// Fused Multiply-Add
	bool HasFMA : 1;

	// 16-bit Floating-Point conversion
	bool HasF16C : 1;

	// Return the Count of Number of Bits Set to 1
	bool HasPOPCNT : 1;

	// Count the Number of Leading Zero Bits
	bool HasLZCNT : 1;

	bool Padding : 7;
} CPUInfo;

bool CPU_InitInfo(CPUInfo* pInfo);
HG_API CPUInfo* CPU_GetInfo();

///////////////////////////////////////////////////////////////////////////////////
// DRAM
///////////////////////////////////////////////////////////////////////////////////
typedef struct DRAMInfo
{
	uint64 TotalSize;
	uint64 AvailableSize;
} DRAMInfo;

bool DRAM_InitInfo(DRAMInfo* pInfo);
HG_API DRAMInfo* DRAM_GetInfo();

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

HG_API bool Power_UpdateStatus(PowerInfo* pInfo);
HG_API PowerInfo* Power_GetInfo();

///////////////////////////////////////////////////////////////////////////////////
// Monitor
///////////////////////////////////////////////////////////////////////////////////
typedef struct MonitorInfo
{
#ifdef _WIN32
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

bool Monitor_InitInfo(MonitorInfo* pInfo, uint32& monitorCount);
HG_API MonitorInfo* Monitor_GetInfo(uint32 monitorIndex);
HG_API uint32 Monitor_GetCount();

///////////////////////////////////////////////////////////////////////////////////
// Time
///////////////////////////////////////////////////////////////////////////////////
typedef struct TimeInfo
{
	int64 Frequency;
	float SecondsPerCycle;
} TimeInfo;

bool Time_Init(TimeInfo* pInfo);
HG_API TimeInfo* Time_GetInfo();
HG_API int64 Time_QueryCounter();

///////////////////////////////////////////////////////////////////////////////////
// Console
///////////////////////////////////////////////////////////////////////////////////
typedef struct ConsoleInfo
{
	void* OutputHandle;
	void* WindowHandle;
} ConsoleInfo;

bool Console_Init(ConsoleInfo* pInfo);
void Console_Shutdown(ConsoleInfo* pInfo);
HG_API void Console_Show(ConsoleInfo* pInfo);
HG_API void Console_Hide(ConsoleInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// Log
///////////////////////////////////////////////////////////////////////////////////
typedef struct LogInfo
{
} LogInfo;
HG_API bool Log_Init(LogInfo* pInfo);

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

bool Thread_Init();
HG_API bool Thread_IsInMainThread();
HG_API uint32 Thread_GetCurrentID();
HG_API const char* Thread_GetCurrentName();
HG_API void Thread_SetCurrentName(const char* pName);
HG_API void Thread_Sleep(uint32 seconds);
HG_API bool Thread_Create(ThreadInfo* pInfo);
HG_API void Thread_Join(ThreadInfo* pInfo);
HG_API void Thread_Detach(ThreadInfo* pInfo);
