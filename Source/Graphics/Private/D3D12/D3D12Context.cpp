/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "D3D12Context.h"

#include "Containers/Vector.h"
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

void Graphics_EnumAdapters(GraphicsContext* pContext, uint32* pAdapterCount, D3D12AdapterInfo* pAdaptersInfo)
{
	constexpr D3D_FEATURE_LEVEL FeatureLevelsRange[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	constexpr uint32 FeatureLevelsCount = sizeof(FeatureLevelsRange) / sizeof(D3D_FEATURE_LEVEL);
	constexpr D3D_FEATURE_LEVEL MinFeatureLevel = FeatureLevelsRange[FeatureLevelsCount - 1];

	uint32 availableAdpaterCount = 0;
	uint32 adapterIndex = 0;
	hg::RefCountPtr<IDXGIAdapter4> pAdapter;
	while (true)
	{
		if (D3D12_FAILED(pContext->Factory->EnumAdapterByGpuPreference(adapterIndex++, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pAdapter))))
		{
			break;
		}

		DXGI_ADAPTER_DESC3 adapterDesc = {};
		pAdapter->GetDesc3(&adapterDesc);
		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		hg::RefCountPtr<ID3D12Device5> pDevice;
		if (D3D12_FAILED(Graphics_D3D12CreateDevice(pAdapter, MinFeatureLevel, IID_PPV_ARGS(&pDevice))))
		{
			continue;
		}

		if (pAdaptersInfo)
		{
			auto& adapterInfo = pAdaptersInfo[availableAdpaterCount];

			D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelsData = {};
			featureLevelsData.pFeatureLevelsRequested = FeatureLevelsRange;
			featureLevelsData.NumFeatureLevels = FeatureLevelsCount;
			D3D12_VERIFY(pDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelsData, sizeof(featureLevelsData)));

			D3D12_FEATURE_DATA_SHADER_MODEL shaderModelData = {};
			shaderModelData.HighestShaderModel = D3D_SHADER_MODEL_6_6;
			D3D12_VERIFY(pDevice->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModelData, sizeof(shaderModelData)));

			adapterInfo.Adapter = pAdapter;
			adapterInfo.DeviceID = adapterDesc.DeviceId;
			adapterInfo.VendorID = adapterDesc.VendorId;
			adapterInfo.Revision = adapterDesc.Revision;
			adapterInfo.DedicatedVRAM = adapterDesc.DedicatedVideoMemory;
			adapterInfo.MaxFeatureLevel = featureLevelsData.MaxSupportedFeatureLevel;
			adapterInfo.MaxShaderModel = shaderModelData.HighestShaderModel;
		}
		else
		{
			++availableAdpaterCount;
		}
	}

	*pAdapterCount = availableAdpaterCount;
}

void Graphics_InitDevice(GraphicsContext* pContext)
{
	uint32 adapterCount = 0;
	Graphics_EnumAdapters(pContext, &adapterCount, nullptr);
	Assert(adapterCount > 0);

	hg::Vector<D3D12AdapterInfo> adaptersInfo(adapterCount);
	Graphics_EnumAdapters(pContext, &adapterCount, adaptersInfo.data());

	uint32 currentAdapterIndex = 0;
	uint32 selectAdapterIndex = 0;
	uint64 maxVRAM = 0;
	for (const auto& adapterInfo : adaptersInfo)
	{
		if (adapterInfo.DedicatedVRAM > maxVRAM)
		{
			maxVRAM = adapterInfo.DedicatedVRAM;
			selectAdapterIndex = currentAdapterIndex;
		}
		++currentAdapterIndex;
	}

	const auto& selectAdapterInfo = adaptersInfo[selectAdapterIndex];
	D3D12_VERIFY(Graphics_D3D12CreateDevice(selectAdapterInfo.Adapter, selectAdapterInfo.MaxFeatureLevel, IID_PPV_ARGS(&pContext->Device)));

#if defined(HG_GFX_ENABLE_DEBUG)
	hg::RefCountPtr<ID3D12InfoQueue1> pInfoQueue;
	if (D3D12_SUCCEED(pContext->Device->QueryInterface(IID_PPV_ARGS(&pContext->InfoQueue))))
	{
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_INFO, FALSE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_MESSAGE, FALSE));

		auto MessageCallback = [](D3D12_MESSAGE_CATEGORY Category, D3D12_MESSAGE_SEVERITY Severity,
			D3D12_MESSAGE_ID ID, LPCSTR pDescription, void* pContext)
			{
				printf("D3D12 Validation Layer: %s\n", pDescription);
			};

		DWORD callbackCookie = 0;
		D3D12_VERIFY(pContext->InfoQueue->RegisterMessageCallback(MessageCallback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nullptr, &callbackCookie));
	}
#endif

	D3D12MA::ALLOCATOR_DESC desc = {};
	desc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
	desc.pDevice = pContext->Device;
	desc.pAdapter = selectAdapterInfo.Adapter;
	D3D12_VERIFY(D3D12MA::CreateAllocator(&desc, &pContext->ResourceAllocator));
}

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
	pDebug1->SetEnableGPUBasedValidation(createInfo.Debug.EnableGPUBasedValidation);
	pDebug1->SetEnableSynchronizedCommandQueueValidation(createInfo.Debug.EnableSynchronizedCommandQueueValidation);
#endif

	D3D12_VERIFY(Graphics_CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&pContext->Factory)));
	Graphics_InitDevice(pContext);
	return true;
}

void Graphics_Shutdown(GraphicsContext* pContext)
{
	if (pContext->Factory)
	{
		pContext->Factory->Release();
		pContext->Factory = nullptr;
	}
	
	if (pContext->Device)
	{
		pContext->Device->Release();
		pContext->Device = nullptr;
	}

#if defined(HG_GFX_ENABLE_DEBUG)
	if (pContext->Debug)
	{
		pContext->Debug->Release();
		pContext->Debug = nullptr;
	}
	
	if (pContext->InfoQueue)
	{
		pContext->InfoQueue->Release();
		pContext->InfoQueue = nullptr;
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