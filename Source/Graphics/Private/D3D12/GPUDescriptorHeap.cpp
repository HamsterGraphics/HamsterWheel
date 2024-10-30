/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "GPUDescriptorHeap.h"

namespace hg
{

GPUDescriptorHeap::GPUDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 descriptorCount) :
	m_type(heapType),
	m_descriptorCount(descriptorCount)
{
	Assert(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV == heapType || D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER == heapType);

	m_incrementSize = pDevice->GetDescriptorHandleIncrementSize(heapType);

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = heapType;
	desc.NumDescriptors = descriptorCount;
	D3D12_VERIFY(pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap)));

	m_cpuStart = m_heap->GetCPUDescriptorHandleForHeapStart();
	m_gpuStart = m_heap->GetGPUDescriptorHandleForHeapStart();
}

}