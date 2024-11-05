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
#include "Base/NameOf.h"
#include "Base/PlatformDefines.h"
#include "Base/RefCountPtr.h"
#include "Containers/Vector.h"
#include "IOperatingSystem.h"

#include <cstdlib>
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
#define MAX_FRAME_COUNT 2

#if defined(HG_GFX_BACKEND_D3D12)
#include "D3D12Config.h"
#elif defined(HG_GFX_BACKEND_METAL)
#include "MetalConfig.h"
#endif

///////////////////////////////////////////////////////
// API Types
///////////////////////////////////////////////////////
struct NullResources;

namespace hg
{
class CPUDescriptorHeap;
class CommandQueue;
class GPUDescriptorHeap;
class Fence;
class SwapChain;
}

typedef struct SwapChainDesc
{
	void* NativeWindowHandle;
	hg::CommandQueue* PresentQueue;
	uint32 BufferCount;
	uint32 BufferWidth;
	uint32 BufferHeight;
	RGBA ClearValue;
	bool EnableVSync;
} SwapChainDesc;

typedef struct GraphicsContextCreateInfo
{
#if defined(HG_GFX_BACKEND_D3D12)
	D3D_FEATURE_LEVEL FeatureLevel;
	bool EnableStablePowerMode;
#if defined(HG_GFX_ENABLE_DEBUG)
	GraphicsDebugContextCreateInfo Debug;
#endif
	void* NativeWindowHandle;
	uint32 BackBufferWidth;
	uint32 BackBufferHeight;

#endif
} GraphicsContextCreateInfo;

typedef struct GraphicsContext
{
#if defined(HG_GFX_BACKEND_D3D12)
	hg::RefCountPtr<IDXGIFactory6> Factory;
	hg::RefCountPtr<ID3D12Device> Device;
	hg::SwapChain* SwapChain;
	
	// Debug
#if defined(HG_GFX_ENABLE_DEBUG)
	hg::RefCountPtr<ID3D12Debug> Debug;
	hg::RefCountPtr<ID3D12InfoQueue1> InfoQueue;
	DWORD CallbackCookie;
#endif

	// Resource
	D3D12MA::Allocator* ResourceAllocator;
	NullResources* NullResources;

	// Descriptor
	hg::CPUDescriptorHeap* CPUDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
	hg::GPUDescriptorHeap* GPUViewDescriptorHeap;
	hg::GPUDescriptorHeap* GPUSamplerDescriptorHeap;
	
	// Command
	hg::RefCountPtr<ID3D12CommandAllocator> CommandAllocator;
	hg::RefCountPtr<ID3D12GraphicsCommandList1> GraphicsCommandList;
	hg::CommandQueue* GraphicsQueue;

	// Synchronization
	hg::Fence* FrameFence;
	uint64 CurrentCPUFrame = 0;
	uint64 CurrentGPUFrame = 0;

#endif
} GraphicsContext;
