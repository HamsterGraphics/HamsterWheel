/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Base/APIDefines.h"
#include "Base/Assert.h"
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

#if defined(HG_GFX_BACKEND_D3D12)
#include <d3d12.h>
#include <dxgi1_6.h>

#if defined(HG_GFX_ENABLE_DEBUG)
#include <dxgidebug.h>
#endif

#define D3D12_SUCCEED(result) (HRESULT)result >= 0
#define D3D12_VERIFY(result) Assert((HRESULT)result >= 0)

typedef struct D3D12GraphicsContextCreateInfo
{
	D3D_FEATURE_LEVEL FeatureLevel;
#if defined(HG_GFX_ENABLE_DEBUG)
	bool EnableGPUBasedValidation;
	bool EnableSynchronizedCommandQueueValidation;
#endif
} D3D12GraphicsContextCreateInfo;

typedef struct D3D12GraphicsContext
{
	IDXGIFactory6* Factory;
#if defined(HG_GFX_ENABLE_DEBUG)
	ID3D12Debug* Debug;
#endif
} D3D12GraphicsContext;

typedef D3D12GraphicsContextCreateInfo GraphicsContextCreateInfo;
typedef D3D12GraphicsContext GraphicsContext;
#elif defined(HG_GFX_BACKEND_VULKAN)

#elif defined(HG_GFX_BACKEND_METAL)

#endif

#define MAX_GPU_COUNT 4
