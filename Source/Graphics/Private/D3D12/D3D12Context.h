/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "GraphicsConfig.h"

HRESULT WINAPI Graphics_D3D12CreateDevice(_In_opt_ void* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, _In_ REFIID riid, _COM_Outptr_opt_ void** ppDevice);
HRESULT WINAPI Graphics_D3D12GetDebugInterface(_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug);
HRESULT WINAPI Graphics_D3D12EnableExperimentalFeatures(UINT NumFeatures, _In_count_(NumFeatures) const IID* pIIDs, _In_opt_count_(NumFeatures) void* pConfigurationStructs, _In_opt_count_(NumFeatures) UINT* pConfigurationStructSizes);
HRESULT WINAPI Graphics_D3D12SerializeVersionedRootSignature(_In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignature, _Out_ ID3DBlob** ppBlob, _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob);
HRESULT WINAPI Graphics_CreateDXGIFactory2(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory);
HRESULT WINAPI Graphics_DXGIGetDebugInterface1(UINT Flags, REFIID riid, _COM_Outptr_ void** pDebug);

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

///////////////////////////////////////////////////////
// Adapter
///////////////////////////////////////////////////////
typedef struct AdapterInfo
{
	hg::RefCountPtr<IDXGIAdapter4> Adapter;
	D3D_FEATURE_LEVEL MaxFeatureLevel;
	D3D_SHADER_MODEL MaxShaderModel;
	uint32 VendorID;
	uint32 DeviceID;
	uint32 Revision;
	uint64 DedicatedVRAM;
	char Name[MAX_GPU_NAME_LENGTH];
} D3D12GPUInfo;

///////////////////////////////////////////////////////
// DescriptorHeap
///////////////////////////////////////////////////////
typedef struct DescriptorHeap
{
	ID3D12DescriptorHeap* Heap;
	ID3D12Device* Device;
	D3D12_CPU_DESCRIPTOR_HANDLE CPUStart;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUStart;
	D3D12_DESCRIPTOR_HEAP_TYPE HeapType;
	uint32 IncrementSize;
	Mutex HeapMutex;
} DescriptorHeap;

void DescriptorHeap_Init(DescriptorHeap* pDescriptorHeap, ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC& desc);