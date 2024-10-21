/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

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