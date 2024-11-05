/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "IGraphics.h"

HRESULT WINAPI Graphics_D3D12CreateDevice(_In_opt_ void* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, _In_ REFIID riid, _COM_Outptr_opt_ void** ppDevice);
HRESULT WINAPI Graphics_D3D12GetDebugInterface(_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug);
HRESULT WINAPI Graphics_D3D12EnableExperimentalFeatures(UINT NumFeatures, _In_count_(NumFeatures) const IID* pIIDs, _In_opt_count_(NumFeatures) void* pConfigurationStructs, _In_opt_count_(NumFeatures) UINT* pConfigurationStructSizes);
HRESULT WINAPI Graphics_D3D12SerializeVersionedRootSignature(_In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignature, _Out_ ID3DBlob** ppBlob, _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob);
HRESULT WINAPI Graphics_CreateDXGIFactory2(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory);
HRESULT WINAPI Graphics_DXGIGetDebugInterface1(UINT Flags, REFIID riid, _COM_Outptr_ void** pDebug);

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
// NullResources
///////////////////////////////////////////////////////
typedef struct NullResources
{
	uint32 TextureSRV2D;
	uint32 TextureUAV2D;
	uint32 BufferSRV;
	uint32 BufferUAV;
	uint32 BufferCBV;
	uint32 Sampler;
} NullResources;