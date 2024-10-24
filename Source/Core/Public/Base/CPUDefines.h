/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "APIDefines.h"
#include "BasicTypes.h"

///////////////////////////////////////////////////////
// Architecture
///////////////////////////////////////////////////////
#if defined(_M_IX86) || defined(__i386__)
#define HG_CPU_ARCH_X86
#define HG_CPU_ARCH_X86_32
#elif defined(_M_X64) || defined(__x86_64__)
#define HG_CPU_ARCH_X86
#define HG_CPU_ARCH_X86_64
#elif defined(_M_ARM) || defined(__arm__)
#define HG_CPU_ARCH_ARM
#elif defined(_M_ARM64) || defined(__aarch64__)
#define HG_CPU_ARCH_AARCH64
#endif

///////////////////////////////////////////////////////
// Brand
///////////////////////////////////////////////////////
#define HG_CPU_VENDOR_AMD "AuthenticAMD"
#define HG_CPU_VENDOR_INTEL "GenuineIntel"

///////////////////////////////////////////////////////
// ISA Feature
///////////////////////////////////////////////////////
typedef struct X86Features
{
	uint32 FMA3 : 1; // Fused Multiply-Add. 3 registers.
	uint32 F16C : 1; // 16-bit Floating-Point conversion
	uint32 LZCNT : 1; // Count the Number of Leading Zero Bits. Called ABM for AMD, LZCNT for Intel
	uint32 POPCNT : 1; // Return the Count of Number of Bits Set to 1

	uint32 SSE : 1;
	uint32 SSE2 : 1;
	uint32 SSE3 : 1;
	uint32 SSSE3 : 1;
	uint32 SSE41 : 1;
	uint32 SSE42 : 1;
	uint32 SSE4a : 1;

	uint32 AVX : 1;
	uint32 AVX2 : 1;
	uint32 AVX512CD : 1;
	uint32 AVX512ER : 1;
	uint32 AVX512F : 1;
	uint32 AVX512PF : 1;
} X86Features;

// https://marcin.juszkiewicz.com.pl/2022/11/08/from-a-diary-of-aarch64-porter-arm-cpu-features-table/
typedef struct AARCH64Features
{
	// ARM v8.0
	uint32 FP : 1;
	uint32 ASIMD : 1; // Advanced SIMD
	uint32 EVTSTRM : 1; // Timer Event Stream Generation
	uint32 CPUID : 1;
	uint32 AES : 1; // Advanced Encryption Standard
	uint32 CRC32 : 1;
	uint32 PMULL : 1;
	uint32 SHA1 : 1;
	uint32 SHA2 : 1;

	// ARM v8.1
	uint32 ASIMDRDM : 1; // Rounding Double Multiply Accumulate
	uint32 ATOMICS : 1;

	// ARM v8.2
	uint32 ASIMDDP : 1; // Dot Product
	uint32 ASIMDFHM : 1; // Floating-point Half-precision Multiplication
	uint32 ASIMDHP : 1; // ASIMD BFloat16
	uint32 BF16 : 1; // BFloat16

	// ARM v8.3
	// ARM v8.4
	// ARM v8.5
	// ARM v8.6
	// ARM v8.7
	// ARM v9.0
	// ARM v9.2
	uint32 EBF16 : 1; // Extended BFloat16
	uint32 SME : 1; // Scalable Matrix Extension
	uint32 SMEB16F32 : 1; // accumulate BFloat16 outer products into FP32 single-precision floating-point tiles
	uint32 SMEF16F32 : 1; // accumulate FP16 half-precision floating-point outer products into FP32 single-precision floating-point tiles
	uint32 SMEF32F32 : 1; // accumulate FP32 single - precision floating - point outer products into single-precision floating-point tiles
	uint32 SMEF64F64 : 1; // accumulate into FP64 double-precision floating-point elements in the ZA array
	uint32 SMEFA64 : 1; // Full Streaming SVE mode instructions
	uint32 SMEI8I32 : 1; // accumulate 8-bit integer outer products into 32-bit integer tiles
	uint32 SMEI16I64 : 1; // accumulate into 64-bit integer elements in the ZA array
	uint32 SVEEBF16 : 1; // Extended BFloat16 instructions (SVE)

	// ARM v10
} AARCH64Features;