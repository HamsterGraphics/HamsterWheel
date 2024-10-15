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
#define HG_API API_EXPORT
#else
#define HG_API API_IMPORT
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define HG_WINDOW_CLASS_NAME L"HamsterGraphics"