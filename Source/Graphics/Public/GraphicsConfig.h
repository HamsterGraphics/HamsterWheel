/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Base/APIDefines.h"
#include "Base/BasicTypes.h"
#include "Base/PlatformDefines.h"

#if defined(HG_GFX_EXPORT)
#define HG_GFX_API API_EXPORT
#else
#if defined(HG_MODULE_SHARED)
#define HG_GFX_API API_IMPORT
#else
#define HG_GFX_API
#endif
#endif

#if defined(HG_PLATFORM_WINDOWS)
#include <d3d12.h>
#endif

#define MAX_GPU_COUNT 4
