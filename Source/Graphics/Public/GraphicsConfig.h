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
#define HG_GFX_API API_EXPORT
#else
#define HG_GFX_API API_IMPORT
#endif

// D3D12
#include <d3d12.h>

#define MAX_GPU_COUNT 4