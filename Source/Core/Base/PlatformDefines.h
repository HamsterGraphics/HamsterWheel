/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#if defined(__ANDROID__)
#define HG_PLATFORM_ANDROID
#elif defined(_WIN64) || defined(_WIN32)
#define HG_PLATFORM_WINDOWS
#elif defined(__apple__) || defined(__APPLE__) || defined(__MACH__)
// From https://stackoverflow.com/a/49560690
#include "TargetConditionals.h"
#if defined(TARGET_OS_OSX)
#define HG_PLATFORM_MACOS
#endif
#if defined(TARGET_OS_IPHONE)
// This is set for any non-Mac Apple products (IOS, TV, WATCH)
#define HG_PLATFORM_IPHONE
#endif
#endif