/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include <Base/APIDefines.h>
#include <Base/BasicTypes.h>

#ifdef HG_GFX_EXPORT
#define HG_GFX_API API_EXPORT
#else
#ifdef HG_MODULE_SHARED
#define HG_GFX_API API_IMPORT
#else
#define HG_GFX_API
#endif
#endif

// D3D12
#include <d3d12.h>

#define MAX_GPU_COUNT 4