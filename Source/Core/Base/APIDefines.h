/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "CompilerDefines.h"

#if defined(HG_COMPILER_MSC)
#define API_EXPORT __declspec(dllexport)
#define API_IMPORT __declspec(dllimport)
#elif defined(HG_COMPILER_GCC)
#define API_EXPORT __attribute__((visibility("default")))
#define API_IMPORT
#else
#define API_EXPORT
#define API_IMPORT
#endif

#if defined(__cplusplus)
#define C_ABI extern "C"
#else
#define C_ABI
#endif

#define HG_CALLDECL __cdecl
