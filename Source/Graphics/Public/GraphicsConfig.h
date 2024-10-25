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

#define D3D12_SUCCEED(result) (HRESULT)result >= 0
#define D3D12_FAILED(result) (HRESULT)result < 0

inline std::string GetErrorString(HRESULT errorCode, ID3D12Device* pDevice)
{
	std::string str;
	char* errorMsg;
	if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&errorMsg, 0, nullptr) != 0)
	{
		str += errorMsg;
		LocalFree(errorMsg);
	}

	return str;
}

inline void LogHRESULT(HRESULT result, ID3D12Device* pDevice, const char* pCode, const char* pFileName, uint32 lineNumber)
{
	if (D3D12_FAILED(result))
	{
		LOG_FATAL("%s:%d: %s - %s", pFileName, lineNumber, GetErrorString(result, pDevice).c_str(), pCode);
	}
}

#define D3D12_VERIFY(result) LogHRESULT(result, nullptr, #result, __FILE__, __LINE__)

namespace D3D12MA
{

class Allocator;

}

typedef struct GraphicsContextCreateInfo
{
	D3D_FEATURE_LEVEL FeatureLevel;
	bool EnableStablePowerMode;
#if defined(HG_GFX_ENABLE_DEBUG)
	GraphicsDebugContextCreateInfo Debug;
#endif
} GraphicsContextCreateInfo;

typedef struct GraphicsContext
{
	IDXGIFactory6* Factory;
	ID3D12Device* Device;
	D3D12MA::Allocator* ResourceAllocator;
#if defined(HG_GFX_ENABLE_DEBUG)
	ID3D12Debug* Debug;
	ID3D12InfoQueue1* InfoQueue;
	DWORD CallbackCookie;
#endif
} GraphicsContext;

#elif defined(HG_GFX_BACKEND_VULKAN)
///////////////////////////////////////////////////////
// Vulkan
///////////////////////////////////////////////////////
typedef struct GraphicsContextCreateInfo
{
} GraphicsContextCreateInfo;

typedef struct GraphicsContext
{
} GraphicsContext;

#elif defined(HG_GFX_BACKEND_METAL)
///////////////////////////////////////////////////////
// Metal
///////////////////////////////////////////////////////
typedef struct GraphicsContextCreateInfo
{
} GraphicsContextCreateInfo;

typedef struct GraphicsContext
{
} GraphicsContext;

#endif
