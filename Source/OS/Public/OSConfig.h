/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Base/APIDefines.h>
#include <Base/BasicTypes.h>

#ifdef HG_EXPORT
#define HG_OS_API API_EXPORT
#else
#define HG_OS_API API_IMPORT
#endif

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#endif

#define HG_WINDOW_CLASS_NAME L"HamsterGraphics"

typedef struct Rect
{
	int32 X;
	int32 Y;
	int32 Width;
	int32 Height;
} Rect;

#define MAX_MONITOR_COUNT 4
#define MAX_THREAD_NAME_LENGTH 32
