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
typedef struct ISAFeatures
{
#if defined(HG_CPU_ARCH_X86)
	uint32 FMA3		: 1; // Fused Multiply-Add. 3 registers.
	uint32 F16C		: 1; // 16-bit Floating-Point conversion
	uint32 LZCNT	: 1; // Count the Number of Leading Zero Bits. Called ABM for AMD, LZCNT for Intel
	uint32 POPCNT	: 1; // Return the Count of Number of Bits Set to 1

	uint32 SSE		: 1;
	uint32 SSE2		: 1;
	uint32 SSE3		: 1;
	uint32 SSSE3	: 1;
	uint32 SSE41	: 1;
	uint32 SSE42	: 1;

	uint32 AVX		: 1;
	uint32 AVX2		: 1;
	uint32 AVX512CD	: 1;
	uint32 AVX512ER	: 1;
	uint32 AVX512F	: 1;
	uint32 AVX512PF	: 1;
#elif defined(HG_CPU_ARCH_AARCH64)
	// https://marcin.juszkiewicz.com.pl/2022/11/08/from-a-diary-of-aarch64-porter-arm-cpu-features-table/
	// ARM v8.0
	uint32 FP			: 1;
	uint32 ASIMD		: 1; // Advanced SIMD
	uint32 EVTSTRM		: 1; // Timer Event Stream Generation
	uint32 CPUID		: 1;
	uint32 AES			: 1; // Advanced Encryption Standard
	uint32 CRC32		: 1;
	uint32 PMULL		: 1;
	uint32 SHA1			: 1;
	uint32 SHA2			: 1;

	// ARM v8.1
	uint32 ASIMDRDM		: 1; // Rounding Double Multiply Accumulate
	uint32 ATOMICS		: 1;

	// ARM v8.2
	uint32 ASIMDDP		: 1; // Dot Product
	uint32 ASIMDFHM		: 1; // Floating-point Half-precision Multiplication
	uint32 ASIMDHP		: 1; // ASIMD BFloat16
	uint32 BF16			: 1; // BFloat16

	// ARM v8.3
	// ARM v8.4
	// ARM v8.5
	// ARM v8.6
	// ARM v8.7
	// ARM v9.0
	// ARM v9.2
	uint32 EBF16		: 1; // Extended BFloat16
	uint32 SME			: 1; // Scalable Matrix Extension
	uint32 SMEB16F32	: 1; // accumulate BFloat16 outer products into FP32 single-precision floating-point tiles
	uint32 SMEF16F32	: 1; // accumulate FP16 half-precision floating-point outer products into FP32 single-precision floating-point tiles
	uint32 SMEF32F32	: 1; // accumulate FP32 single - precision floating - point outer products into single-precision floating-point tiles
	uint32 SMEF64F64	: 1; // accumulate into FP64 double-precision floating-point elements in the ZA array
	uint32 SMEFA64		: 1; // Full Streaming SVE mode instructions
	uint32 SMEI8I32		: 1; // accumulate 8-bit integer outer products into 32-bit integer tiles
	uint32 SMEI16I64	: 1; // accumulate into 64-bit integer elements in the ZA array
	uint32 SVEEBF16		: 1; // Extended BFloat16 instructions (SVE)

	// ARM v10
#endif
} ISAFeatures;

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

C_ABI HG_OS_API bool HG_CALLDECL CPU_InitInfo(CPUInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// DRAM
///////////////////////////////////////////////////////////////////////////////////
typedef struct DRAMInfo
{
	uint64 TotalSize;
	uint64 AvailableSize;
} DRAMInfo;

C_ABI HG_OS_API bool HG_CALLDECL DRAM_InitInfo(DRAMInfo* pInfo);

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

C_ABI HG_OS_API bool HG_CALLDECL Power_UpdateStatus(PowerInfo* pInfo);

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

C_ABI HG_OS_API bool HG_CALLDECL Monitor_InitInfo(MonitorInfo* pInfo, uint32& monitorCount);

///////////////////////////////////////////////////////////////////////////////////
// Time
///////////////////////////////////////////////////////////////////////////////////
typedef struct TimeInfo
{
	int64 Frequency;
	float SecondsPerCycle;
} TimeInfo;

C_ABI HG_OS_API bool HG_CALLDECL Time_Init(TimeInfo* pInfo);
C_ABI HG_OS_API int64 HG_CALLDECL Time_QueryCounter();

///////////////////////////////////////////////////////////////////////////////////
// Console
///////////////////////////////////////////////////////////////////////////////////
typedef struct ConsoleInfo
{
	void* OutputHandle;
	void* WindowHandle;
} ConsoleInfo;

C_ABI HG_OS_API bool HG_CALLDECL Console_Init(ConsoleInfo* pInfo);
C_ABI HG_OS_API void HG_CALLDECL Console_Shutdown(ConsoleInfo* pInfo);
C_ABI HG_OS_API void HG_CALLDECL Console_Show(ConsoleInfo* pInfo);
C_ABI HG_OS_API void HG_CALLDECL Console_Hide(ConsoleInfo* pInfo);

///////////////////////////////////////////////////////////////////////////////////
// Environment Variable
///////////////////////////////////////////////////////////////////////////////////
C_ABI HG_OS_API bool HG_CALLDECL EnvironmentVariable_Contains(const char* key);
C_ABI HG_OS_API const char* HG_CALLDECL EnvironmentVariable_Get(const char* key);
C_ABI HG_OS_API bool HG_CALLDECL EnvironmentVariable_Set(const char* key, const char* value);

///////////////////////////////////////////////////////////////////////////////////
// Module
///////////////////////////////////////////////////////////////////////////////////
C_ABI HG_OS_API void* HG_CALLDECL Module_Load(const char* pFilePath);
C_ABI HG_OS_API void HG_CALLDECL Module_Unload(void* pModule);
C_ABI HG_OS_API void* HG_CALLDECL Module_GetFunctionAddress(void* pModule, const char* pFunctionName);

///////////////////////////////////////////////////////////////////////////////////
// Log
///////////////////////////////////////////////////////////////////////////////////
typedef struct LogInfo
{
} LogInfo;

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

C_ABI HG_OS_API uint32 HG_CALLDECL Thread_GetCurrentID();
C_ABI HG_OS_API const char* HG_CALLDECL Thread_GetCurrentName();
C_ABI HG_OS_API void HG_CALLDECL Thread_SetCurrentName(const char* pName);
C_ABI HG_OS_API void HG_CALLDECL Thread_Sleep(uint32 seconds);
C_ABI HG_OS_API bool HG_CALLDECL Thread_Create(ThreadInfo* pInfo);
C_ABI HG_OS_API void HG_CALLDECL Thread_Join(ThreadInfo* pInfo);
C_ABI HG_OS_API void HG_CALLDECL Thread_Detach(ThreadInfo* pInfo);
