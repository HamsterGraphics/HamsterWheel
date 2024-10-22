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
	int32 FMA3 : 1; // Fused Multiply-Add. 3 registers.
	int32 FMA4 : 1; // Fused Multiply-Add. 4 registers.
	int32 F16C : 1; // 16-bit Floating-Point conversion
	int32 LZCNT : 1; // Count the Number of Leading Zero Bits. Called ABM for AMD, LZCNT for Intel
	int32 POPCNT : 1; // Return the Count of Number of Bits Set to 1

	int32 SSE : 1;
	int32 SSE2 : 1;
	int32 SSE3 : 1;
	int32 SSSE3 : 1;
	int32 SSE41 : 1;
	int32 SSE42 : 1;
	int32 SSE4a : 1;

	int32 AVX : 1;
	int32 AVX2 : 1;
	int32 AVX512CD : 1;
	int32 AVX512ER : 1;
	int32 AVX512F : 1;
	int32 AVX512PF : 1;
} X86Features;

// https://marcin.juszkiewicz.com.pl/2022/11/08/from-a-diary-of-aarch64-porter-arm-cpu-features-table/
typedef struct AARCH64Features
{
	// ARM v8.0
	int32 FP : 1;
	int32 ASIMD : 1; // Advanced SIMD
	int32 EVTSTRM : 1; // Timer Event Stream Generation
	int32 CPUID : 1;
	int32 AES : 1; // Advanced Encryption Standard
	int32 CRC32 : 1;
	int32 PMULL : 1;
	int32 SHA1 : 1;
	int32 SHA2 : 1;

	// ARM v8.1
	int32 ASIMDRDM : 1; // Rounding Double Multiply Accumulate
	int32 ATOMICS : 1;

	// ARM v8.2
	int32 ASIMDDP : 1; // Dot Product
	int32 ASIMDFHM : 1; // Floating-point Half-precision Multiplication
	int32 ASIMDHP : 1; // ASIMD BFloat16
	int32 BF16 : 1; // BFloat16

	// ARM v8.3
	// ARM v8.4
	// ARM v8.5
	// ARM v8.6
	// ARM v8.7
	// ARM v9.0
	// ARM v9.2
	int32 EBF16 : 1; // Extended BFloat16
	int32 SME : 1; // Scalable Matrix Extension
	int32 SMEB16F32 : 1; // accumulate BFloat16 outer products into FP32 single-precision floating-point tiles
	int32 SMEF16F32 : 1; // accumulate FP16 half-precision floating-point outer products into FP32 single-precision floating-point tiles
	int32 SMEF32F32 : 1; // accumulate FP32 single - precision floating - point outer products into single-precision floating-point tiles
	int32 SMEF64F64 : 1; // accumulate into FP64 double-precision floating-point elements in the ZA array
	int32 SMEFA64 : 1; // Full Streaming SVE mode instructions
	int32 SMEI8I32 : 1; // accumulate 8-bit integer outer products into 32-bit integer tiles
	int32 SMEI16I64 : 1; // accumulate into 64-bit integer elements in the ZA array
	int32 SVEEBF16 : 1; // Extended BFloat16 instructions (SVE)

	// ARM v10
} AARCH64Features;