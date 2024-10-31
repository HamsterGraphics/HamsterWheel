/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "D3D12Context.h"

#include "Base/NameOf.h"
#include "Containers/Vector.h"
#include "CPUDescriptorHeap.h"
#include "GPUDescriptorHeap.h"
#include "IGraphics.h"

#include "D3D12MemoryAllocator/D3D12MemAlloc.h"

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
	LOG_TRACE("Loading d3d12.dll.");
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

	LOG_TRACE("Loading dxgi.dll.");
	g_pDXGIModule = Module_Load("dxgi.dll");
	g_CreateDXGIFactory2 = (PFN_CreateDXGIFactory2)Module_GetFunctionAddress(g_pDXGIModule, "CreateDXGIFactory2");
	g_DXGIGetDebugInterface1 = (PFN_DXGIGetDebugInterface1)Module_GetFunctionAddress(g_pDXGIModule, "DXGIGetDebugInterface1");
	Assert(g_pDXGIModule != nullptr);
}

void UnloadD3D12API()
{
	LOG_TRACE("Unloading d3d12.dll.");
	Module_Unload(g_pD3D12Module);
	g_pD3D12Module = nullptr;
	g_D3D12CreateDevice = nullptr;
	g_D3D12GetDebugInterface = nullptr;
	g_D3D12EnableExperimentalFeatures = nullptr;
	g_D3D12SerializeVersionedRootSignature = nullptr;

	LOG_TRACE("Unloading dxgi.dll.");
	Module_Unload(g_pDXGIModule);
	g_pDXGIModule = nullptr;
	g_CreateDXGIFactory2 = nullptr;
	g_DXGIGetDebugInterface1 = nullptr;
}

#endif

///////////////////////////////////////////////////////
// Adapter
///////////////////////////////////////////////////////
void Graphics_EnumAdapters(GraphicsContext* pContext, uint32* pAdapterCount, AdapterInfo* pAdaptersInfo)
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

///////////////////////////////////////////////////////
// Device
///////////////////////////////////////////////////////
void Graphics_InitDevice(GraphicsContext* pContext)
{
	LOG_TRACE("Collect d3d12 adapter info.");
	uint32 adapterCount = 0;
	Graphics_EnumAdapters(pContext, &adapterCount, nullptr);
	Assert(adapterCount > 0);

	hg::Vector<AdapterInfo> adaptersInfo(adapterCount);
	Graphics_EnumAdapters(pContext, &adapterCount, adaptersInfo.data());

	uint32 currentAdapterIndex = 0;
	uint32 selectAdapterIndex = 0;
	uint64 maxVRAM = 0;
	for (const auto& adapterInfo : adaptersInfo)
	{
		LOG_INFO("[Adapter] %s", adapterInfo.Name);
		LOG_INFO("  DeviceID = %u", adapterInfo.DeviceID);
		LOG_INFO("  Revision = %u", adapterInfo.Revision);
		LOG_INFO("  VendorID = %u", adapterInfo.VendorID);
		LOG_INFO("  DedicatedVRAM = %.3f MB", adapterInfo.DedicatedVRAM / 1024.f / 1024.f);
		LOG_INFO("  MaxFeatureLevel = 0x%x", adapterInfo.MaxFeatureLevel);
		LOG_INFO("  MaxShaderModel = 0x%x", adapterInfo.MaxShaderModel);
		if (adapterInfo.DedicatedVRAM > maxVRAM)
		{
			maxVRAM = adapterInfo.DedicatedVRAM;
			selectAdapterIndex = currentAdapterIndex;
		}
		++currentAdapterIndex;
	}

	LOG_TRACE("Init d3d12 device.");
	const auto& selectAdapterInfo = adaptersInfo[selectAdapterIndex];
	D3D12_VERIFY(Graphics_D3D12CreateDevice(selectAdapterInfo.Adapter, selectAdapterInfo.MaxFeatureLevel, IID_PPV_ARGS(&pContext->Device)));

#if defined(HG_GFX_ENABLE_DEBUG)
	hg::RefCountPtr<ID3D12InfoQueue1> pInfoQueue;
	if (D3D12_SUCCEED(D3D12Cast(pContext->Device, &pContext->InfoQueue)))
	{
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_INFO, FALSE));
		D3D12_VERIFY(pContext->InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_MESSAGE, FALSE));

		auto MessageCallback = [](D3D12_MESSAGE_CATEGORY category, D3D12_MESSAGE_SEVERITY severity, D3D12_MESSAGE_ID messageID, LPCSTR pDescription, void* pContext)
		{
			if (severity == D3D12_MESSAGE_SEVERITY_CORRUPTION)
			{
				LOG_ERROR("[Corruption] [%d] : %s (%d)", category, pDescription, messageID);
			}
			else if (severity == D3D12_MESSAGE_SEVERITY_ERROR)
			{
				LOG_ERROR("[%d] : %s (%d)", category, pDescription, messageID);
			}
			else if (severity == D3D12_MESSAGE_SEVERITY_WARNING)
			{
				LOG_WARNING("[%d] : %s (%d)", category, pDescription, messageID);
			}
			else if (severity == D3D12_MESSAGE_SEVERITY_INFO)
			{
				LOG_INFO("[%d] : %s (%d)", category, pDescription, messageID);
			}
		};
		D3D12_VERIFY(pContext->InfoQueue->RegisterMessageCallback(MessageCallback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nullptr, &pContext->CallbackCookie));
	}
#endif

	LOG_TRACE("Init d3d12 resource allocator.");
	D3D12MA::ALLOCATOR_DESC desc = {};
	desc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
	desc.pDevice = pContext->Device;
	desc.pAdapter = selectAdapterInfo.Adapter;
	D3D12_VERIFY(D3D12MA::CreateAllocator(&desc, &pContext->ResourceAllocator));
}

///////////////////////////////////////////////////////
// Descriptors
///////////////////////////////////////////////////////
void Graphics_InitDescriptorHeaps(GraphicsContext* pContext)
{
	constexpr uint32 CPUDescriptorConfig[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] = {
		8192, // CBV_SRV_UAV
		2048, // SAMPLER
		512,  // RTV
		512   // DSV
	};

	LOG_TRACE("Init CPU descriptor heap.");
	for (uint32 ii = 0; ii < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++ii)
	{
		D3D12_DESCRIPTOR_HEAP_TYPE heapType = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(ii);
		uint32 descriptorCount = CPUDescriptorConfig[ii];
		pContext->CPUDescriptorHeaps[ii] = new hg::CPUDescriptorHeap(pContext->Device, heapType, descriptorCount);
		LOG_TRACE("  Type = %s, DescriptorCount = %u", hg::EnumName(pContext->CPUDescriptorHeaps[ii]->GetHeapType()).data(), pContext->CPUDescriptorHeaps[ii]->GetDescriptorCount());
	}

	LOG_TRACE("Init GPU descriptor heap.");
	pContext->GPUViewDescriptorHeap = new hg::GPUDescriptorHeap(pContext->Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 8192, 8192);
	LOG_TRACE("  Type = %s, DescriptorCount = %u", hg::EnumName(pContext->GPUViewDescriptorHeap->GetHeapType()).data(), pContext->GPUViewDescriptorHeap->GetDescriptorCount());
	pContext->GPUSamplerDescriptorHeap = new hg::GPUDescriptorHeap(pContext->Device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 2048, 0);
	LOG_TRACE("  Type = %s, DescriptorCount = %u", hg::EnumName(pContext->GPUSamplerDescriptorHeap->GetHeapType()).data(), pContext->GPUSamplerDescriptorHeap->GetDescriptorCount());
}

void Graphics_DestroyDescriptorHeaps(GraphicsContext* pContext)
{
	for (uint32 ii = 0; ii < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++ii)
	{
		delete pContext->CPUDescriptorHeaps[ii];
		pContext->CPUDescriptorHeaps[ii] = nullptr;
	}
	delete pContext->GPUViewDescriptorHeap;
	pContext->GPUViewDescriptorHeap = nullptr;
	delete pContext->GPUSamplerDescriptorHeap;
	pContext->GPUSamplerDescriptorHeap = nullptr;
}

///////////////////////////////////////////////////////
// Resource
///////////////////////////////////////////////////////
void Graphics_InitDefaultResources(GraphicsContext* pContext)
{
}

///////////////////////////////////////////////////////
// Entry point
///////////////////////////////////////////////////////
bool Graphics_Init(const GraphicsContextCreateInfo& createInfo, GraphicsContext* pContext)
{
#if defined(HG_GFX_DYNAMIC_API)
	LoadD3D12API();
#endif

	uint32 factoryFlags = 0;
#if defined(HG_GFX_ENABLE_DEBUG)
	LOG_TRACE("Init d3d12 debug layer.");
	factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
	D3D12_VERIFY(Graphics_D3D12GetDebugInterface(IID_PPV_ARGS(&pContext->Debug)));
	pContext->Debug->EnableDebugLayer();

	hg::RefCountPtr<ID3D12Debug1> pDebug1;
	D3D12_VERIFY(D3D12Cast(pContext->Debug, &pDebug1));
	pDebug1->SetEnableGPUBasedValidation(createInfo.Debug.EnableGPUBasedValidation);
	pDebug1->SetEnableSynchronizedCommandQueueValidation(createInfo.Debug.EnableSynchronizedCommandQueueValidation);
#endif

	LOG_TRACE("Init dxgi factory.");
	D3D12_VERIFY(Graphics_CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&pContext->Factory)));

	Graphics_InitDevice(pContext);

	if (createInfo.EnableStablePowerMode)
	{
		D3D12_VERIFY(Graphics_D3D12EnableExperimentalFeatures(0, nullptr, nullptr, nullptr));
		// In Windows, you need to open Developer mode at first.
		D3D12_VERIFY(pContext->Device->SetStablePowerState(TRUE));
	}

	Graphics_InitDescriptorHeaps(pContext);
	Graphics_InitDefaultResources(pContext);
	Graphics_InitCommandPool(pContext);

	return true;
}

void Graphics_Shutdown(GraphicsContext* pContext)
{
	Graphics_DestroyDescriptorHeaps(pContext);
	Graphics_DestroyCommandPool(pContext);

#if defined(HG_GFX_ENABLE_DEBUG)
	LOG_TRACE("Report d3d12 live objects.");
	hg::RefCountPtr<ID3D12DebugDevice> pDebugDevice;
	D3D12_VERIFY(D3D12Cast<ID3D12DebugDevice>(pContext->Device, &pDebugDevice));
	pDebugDevice->ReportLiveDeviceObjects((D3D12_RLDO_FLAGS)(D3D12_RLDO_DETAIL));

	if (pContext->InfoQueue)
	{
		pContext->InfoQueue->UnregisterMessageCallback(pContext->CallbackCookie);
	}
#endif

#if defined(HG_GFX_DYNAMIC_API)
	UnloadD3D12API();
#endif
}

void Graphics_BeginFrame(GraphicsContext* pContext)
{
}

void Graphics_EndFrame(GraphicsContext* pContext)
{
	pContext->GPUViewDescriptorHeap->EndFrame();
	pContext->GPUSamplerDescriptorHeap->EndFrame();
}

void Graphics_InitCommandQueue(GraphicsContext* pContext)
{
}

void Graphics_DestroyCommandQueue(GraphicsContext* pContext)
{
}

void Graphics_InitCommandPool(GraphicsContext* pContext)
{
	D3D12_VERIFY(pContext->Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pContext->CommandAllocator)));
}

void Graphics_DestroyCommandPool(GraphicsContext* pContext)
{
}

///////////////////////////////////////////////////////
// API
///////////////////////////////////////////////////////
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