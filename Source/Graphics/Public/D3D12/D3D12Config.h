/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

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

namespace hg
{
class CPUDescriptorHeap;
class GPUDescriptorHeap;
}

struct DescriptorHeap;
struct NullResources;

#if defined(HG_GFX_ENABLE_DEBUG)
static const GUID IID_DXGI_DEBUG_ALL = { 0xe48ae283, 0xda80, 0x490b, { 0x87, 0xe6, 0x43, 0xe9, 0xa9, 0xcf, 0xda, 0x89 } };
#endif

template<typename T>
inline HRESULT D3D12Cast(IUnknown* pSource, T** pTarget)
{
	return pSource->QueryInterface(__uuidof(T), reinterpret_cast<void**>(pTarget));
}

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