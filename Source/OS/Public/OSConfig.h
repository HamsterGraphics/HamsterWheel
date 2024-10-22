/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Base/APIDefines.h"
#include "Base/BasicTypes.h"
#include "Base/CPUDefines.h"
#include "Base/PlatformDefines.h"
#include "Math/Math.h"

#if defined(HG_OS_EXPORT)
#define HG_OS_API API_EXPORT
#else
#if defined(HG_MODULE_SHARED)
#define HG_OS_API API_IMPORT
#else
#define HG_OS_API
#endif
#endif

#if defined(HG_PLATFORM_WINDOWS)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define HG_WINDOW_CLASS_NAME L"HamsterGraphics"

#include <windows.h>

#endif

#define MAX_MONITOR_COUNT 4
#define MAX_THREAD_NAME_LENGTH 32