/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "D3D12Context.h"

#include "Base/RefCountPtr.h"
#include "IGraphics.h"
#include "IOperatingSystem.h"

#if defined(HG_GFX_DYNAMIC_API)
typedef HRESULT (WINAPI *PFN_D3D12CreateDevice)(_In_opt_ void* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, _In_ REFIID riid, _COM_Outptr_opt_ void** ppDevice);
typedef HRESULT (WINAPI *PFN_D3D12GetDebugInterface)(_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug);
typedef HRESULT (WINAPI *PFN_D3D12EnableExperimentalFeatures)(UINT NumFeatures, _In_count_(NumFeatures) const IID* pIIDs, _In_opt_count_(NumFeatures) void* pConfigurationStructs, _In_opt_count_(NumFeatures) UINT* pConfigurationStructSizes);
typedef HRESULT (WINAPI *PFN_D3D12SerializeVersionedRootSignature)(_In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignature, _Out_ ID3DBlob** ppBlob, _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob);

static void* g_pD3D12Module;
static PFN_D3D12CreateDevice g_D3D12CreateDevice;
static PFN_D3D12GetDebugInterface g_D3D12GetDebugInterface;
static PFN_D3D12EnableExperimentalFeatures g_D3D12EnableExperimentalFeatures;
static PFN_D3D12SerializeVersionedRootSignature g_D3D12SerializeVersionedRootSignature;

typedef HRESULT (WINAPI *PFN_CreateDXGIFactory2)(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory);
typedef HRESULT (WINAPI *PFN_DXGIGetDebugInterface1)(UINT Flags, REFIID riid, _COM_Outptr_ void** pDebug);

static void* g_pDXGIModule;
static PFN_CreateDXGIFactory2 g_CreateDXGIFactory2;
static PFN_DXGIGetDebugInterface1 g_DXGIGetDebugInterface1;

void LoadD3D12API()
{
	g_pD3D12Module = Module_Load("d3d12.dll");
	Assert(g_pD3D12Module != nullptr);
	g_D3D12CreateDevice = (PFN_D3D12CreateDevice)Module_GetFunctionAddress(g_pD3D12Module, "D3D12CreateDevice");
	g_D3D12GetDebugInterface = (PFN_D3D12GetDebugInterface)Module_GetFunctionAddress(g_pD3D12Module, "D3D12GetDebugInterface");
	g_D3D12EnableExperimentalFeatures = (PFN_D3D12EnableExperimentalFeatures)Module_GetFunctionAddress(g_pD3D12Module, "D3D12EnableExperimentalFeatures");
	g_D3D12SerializeVersionedRootSignature = (PFN_D3D12SerializeVersionedRootSignature)Module_GetFunctionAddress(g_pD3D12Module, "D3D12SerializeVersionedRootSignature");
	Assert(g_D3D12CreateDevice != nullptr);
	Assert(g_D3D12GetDebugInterface != nullptr);
	Assert(g_D3D12EnableExperimentalFeatures != nullptr);
	Assert(g_D3D12SerializeVersionedRootSignature != nullptr);

	g_pDXGIModule = Module_Load("dxgi.dll");
	g_CreateDXGIFactory2 = (PFN_CreateDXGIFactory2)Module_GetFunctionAddress(g_pDXGIModule, "CreateDXGIFactory2");
	g_DXGIGetDebugInterface1 = (PFN_DXGIGetDebugInterface1)Module_GetFunctionAddress(g_pDXGIModule, "DXGIGetDebugInterface1");
	Assert(g_pDXGIModule != nullptr);
}

void UnloadD3D12API()
{
	Module_Unload(g_pD3D12Module);
	g_pD3D12Module = nullptr;
	g_D3D12CreateDevice = nullptr;
	g_D3D12GetDebugInterface = nullptr;
	g_D3D12EnableExperimentalFeatures = nullptr;
	g_D3D12SerializeVersionedRootSignature = nullptr;

	Module_Unload(g_pDXGIModule);
	g_pDXGIModule = nullptr;
	g_CreateDXGIFactory2 = nullptr;
	g_DXGIGetDebugInterface1 = nullptr;
}

#endif

bool Graphics_Init(const GraphicsContextCreateInfo& createInfo, GraphicsContext* pContext)
{
#if defined(HG_GFX_DYNAMIC_API)
	LoadD3D12API();
#endif

	uint32 factoryFlags = 0;
#if defined(HG_GFX_ENABLE_DEBUG)
	factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
	D3D12_VERIFY(Graphics_D3D12GetDebugInterface(IID_PPV_ARGS(&pContext->Debug)));
	pContext->Debug->EnableDebugLayer();

	hg::RefCountPtr<ID3D12Debug1> pDebug1;
	D3D12_VERIFY(pContext->Debug->QueryInterface(IID_PPV_ARGS(&pDebug1)));
	pDebug1->SetEnableGPUBasedValidation(createInfo.EnableGPUBasedValidation);
	pDebug1->SetEnableSynchronizedCommandQueueValidation(createInfo.EnableSynchronizedCommandQueueValidation);
#endif
	D3D12_VERIFY(Graphics_CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&pContext->Factory)));

	return true;
}

void Graphics_Shutdown(GraphicsContext* pContext)
{
	if (pContext->Factory)
	{
		pContext->Factory->Release();
		pContext->Factory = nullptr;
	}
	
#if defined(HG_GFX_ENABLE_DEBUG)
	if (pContext->Debug)
	{
		pContext->Debug->Release();
		pContext->Debug = nullptr;
	}
	
	hg::RefCountPtr<IDXGIDebug1> pDebug1;
	D3D12_VERIFY(Graphics_DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug1)));
	pDebug1->ReportLiveObjects(IID_DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
#endif

#if defined(HG_GFX_DYNAMIC_API)
	UnloadD3D12API();
#endif
}

HRESULT Graphics_D3D12CreateDevice(_In_opt_ void* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, _In_ REFIID riid, _COM_Outptr_opt_ void** ppDevice)
{
#if defined(HG_GFX_DYNAMIC_API)
	return g_D3D12CreateDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);
#else
	return D3D12CreateDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);
#endif
}

HRESULT Graphics_D3D12GetDebugInterface(_In_ REFIID riid, _COM_Outptr_opt_ void** ppvDebug)
{
#if defined(HG_GFX_DYNAMIC_API)
	return g_D3D12GetDebugInterface(riid, ppvDebug);
#else
	return D3D12GetDebugInterface(riid, ppvDebug);
#endif
}

HRESULT Graphics_D3D12EnableExperimentalFeatures(UINT NumFeatures, _In_count_(NumFeatures) const IID* pIIDs, _In_opt_count_(NumFeatures) void* pConfigurationStructs, _In_opt_count_(NumFeatures) UINT* pConfigurationStructSizes)
{
#if defined(HG_GFX_DYNAMIC_API)
	return g_D3D12EnableExperimentalFeatures(NumFeatures, pIIDs, pConfigurationStructs, pConfigurationStructSizes);
#else
	return D3D12EnableExperimentalFeatures(NumFeatures, pIIDs, pConfigurationStructs, pConfigurationStructSizes);
#endif
}

HRESULT Graphics_D3D12SerializeVersionedRootSignature(_In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pRootSignature, _Out_ ID3DBlob** ppBlob, _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorBlob)
{
#if defined(HG_GFX_DYNAMIC_API)
	return g_D3D12SerializeVersionedRootSignature(pRootSignature, ppBlob, ppErrorBlob);
#else
	return D3D12SerializeVersionedRootSignature(pRootSignature, ppBlob, ppErrorBlob);
#endif
}

HRESULT Graphics_CreateDXGIFactory2(UINT Flags, REFIID riid, _COM_Outptr_ void** ppFactory)
{
#if defined(HG_GFX_DYNAMIC_API)
	return g_CreateDXGIFactory2(Flags, riid, ppFactory);
#else
	return CreateDXGIFactory2(Flags, riid, ppFactory);
#endif
}

HRESULT Graphics_DXGIGetDebugInterface1(UINT Flags, REFIID riid, _COM_Outptr_ void** pDebug)
{
#if defined(HG_GFX_DYNAMIC_API)
	return g_DXGIGetDebugInterface1(Flags, riid, pDebug);
#else
	return DXGIGetDebugInterface1(Flags, riid, pDebug);
#endif
}