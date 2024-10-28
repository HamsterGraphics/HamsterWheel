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
#include "Base/RefCountPtr.h"
#include "IOperatingSystem.h"

#include <string>

#if defined(HG_GFX_EXPORT)
#define HG_GFX_API API_EXPORT
#else
#if defined(HG_MODULE_SHARED)
#define HG_GFX_API API_IMPORT
#else
#define HG_GFX_API
#endif
#endif

#define MAX_GPU_COUNT 4
#define MAX_GPU_NAME_LENGTH 128

#if defined(HG_GFX_BACKEND_D3D12)
///////////////////////////////////////////////////////
// D3D12
///////////////////////////////////////////////////////
#include <d3d12.h>
#include <dxgi1_6.h>

#if defined(HG_GFX_ENABLE_DEBUG)
#include <dxgidebug.h>
typedef struct GraphicsDebugContextCreateInfo
{
	bool EnableGPUBasedValidation;
	bool EnableSynchronizedCommandQueueValidation;
} GraphicsDebugContextCreateInfo;
#endif

// Forward Declaration
namespace D3D12MA
{
class Allocator;
}

struct DescriptorHeap;
struct NullResources;

#endif

///////////////////////////////////////////////////////
// API Types
///////////////////////////////////////////////////////
typedef struct GraphicsContextCreateInfo
{
#if defined(HG_GFX_BACKEND_D3D12)
	D3D_FEATURE_LEVEL FeatureLevel;
	bool EnableStablePowerMode;
#if defined(HG_GFX_ENABLE_DEBUG)
	GraphicsDebugContextCreateInfo Debug;
#endif

#endif
} GraphicsContextCreateInfo;

typedef struct GraphicsContext
{
#if defined(HG_GFX_BACKEND_D3D12)
	hg::RefCountPtr<IDXGIFactory6> Factory;
	hg::RefCountPtr<ID3D12Device> Device;
	D3D12MA::Allocator* ResourceAllocator;
	DescriptorHeap* CPUDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
	DescriptorHeap* GPUViewDescriptorHeap;
	DescriptorHeap* GPUSamplerDescriptorHeap;
	NullResources* NullResources;
#if defined(HG_GFX_ENABLE_DEBUG)
	hg::RefCountPtr<ID3D12Debug> Debug;
	hg::RefCountPtr<ID3D12InfoQueue1> InfoQueue;
	DWORD CallbackCookie;
#endif
#endif
} GraphicsContext;
