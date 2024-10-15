/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Config.h"

 ///////////////////////////////////////////////////////////////////////////////////
 // CPU
 ///////////////////////////////////////////////////////////////////////////////////
typedef struct CPUInfo
{
	char Vendor[256];
	char Brand[256];

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