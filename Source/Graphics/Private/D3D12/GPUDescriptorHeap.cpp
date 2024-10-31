/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "GPUDescriptorHeap.h"

namespace hg
{

GPUDescriptorHeap::GPUDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 persistentDescriptorCount, uint32 tempDescriptorCount) :
	m_type(heapType),
	m_heapIndex(0)
{
	Assert(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV == heapType || D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER == heapType);
	m_persistentDescriptorCount = RoundUp(persistentDescriptorCount, 128);
	m_tempDescriptorCount = RoundUp(tempDescriptorCount, 128);
	m_totalDescriptorCount = m_persistentDescriptorCount + m_tempDescriptorCount;
	m_descriptorIDs.resize(m_persistentDescriptorCount);
	std::iota(m_descriptorIDs.begin(), m_descriptorIDs.end(), 0);
	m_incrementSize = pDevice->GetDescriptorHandleIncrementSize(m_type);

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NodeMask = 0; // TODO : multiple gpu
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = m_type;
	desc.NumDescriptors = m_totalDescriptorCount;
	for (int32 heapIndex = 0; heapIndex < MAX_FRAME_COUNT; ++heapIndex)
	{
		D3D12_VERIFY(pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap[heapIndex])));
		m_cpuStart[heapIndex] = m_heap[heapIndex]->GetCPUDescriptorHandleForHeapStart();
		m_gpuStart[heapIndex] = m_heap[heapIndex]->GetGPUDescriptorHandleForHeapStart();
	}
}

void GPUDescriptorHeap::EndFrame()
{
	m_tempDescriptorCount = 0;
	m_heapIndex = (m_heapIndex + 1) % MAX_FRAME_COUNT;
}

GPUDescriptorAllocation GPUDescriptorHeap::Allocate()
{
	uint32 allocateIndex = m_allocatedIndex.fetch_add(1);
	Assert(allocateIndex < m_persistentDescriptorCount);
	uint32 descriptorID = m_descriptorIDs[allocateIndex];

	GPUDescriptorAllocation allocation;
	allocation.CPUHandle = GetCPUHandleFromDescriptorID(descriptorID);
	allocation.GPUHandle = GetGPUHandleFromDescriptorID(descriptorID);
	return allocation;
}

GPUDescriptorAllocation GPUDescriptorHeap::AllocateTemp()
{
	uint32 tempAllocateIndex = m_tempAllocatedIndex.fetch_add(1);
	Assert(tempAllocateIndex < m_tempDescriptorCount);
	uint32 descriptorID = m_persistentDescriptorCount + tempAllocateIndex;
	Assert(descriptorID < m_totalDescriptorCount);

	GPUDescriptorAllocation allocation;
	allocation.CPUHandle = GetCPUHandleFromDescriptorID(descriptorID);
	allocation.GPUHandle = GetGPUHandleFromDescriptorID(descriptorID);
	return allocation;
}

void GPUDescriptorHeap::Free(GPUDescriptorAllocation handle)
{
	uint32 descriptorID = GetDescriptorIDFromHandle(handle.CPUHandle);
	Assert(descriptorID == GetDescriptorIDFromHandle(handle.GPUHandle));
	Assert(descriptorID < m_persistentDescriptorCount);
	uint32 freeIndex = m_allocatedIndex.fetch_sub(1) - 1;
	Assert(freeIndex >= 0);
	m_descriptorIDs[freeIndex] = descriptorID;
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUDescriptorHeap::GetCPUHandleFromDescriptorID(uint32 descriptorID)
{
	return D3D12_CPU_DESCRIPTOR_HANDLE{ .ptr = m_cpuStart[m_heapIndex].ptr + descriptorID * m_incrementSize };
}

uint32 GPUDescriptorHeap::GetDescriptorIDFromHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
	return static_cast<uint32>((handle.ptr - m_cpuStart[m_heapIndex].ptr) / m_incrementSize);
}

D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorHeap::GetGPUHandleFromDescriptorID(uint32 descriptorID)
{
	return D3D12_GPU_DESCRIPTOR_HANDLE{ .ptr = m_gpuStart[m_heapIndex].ptr + descriptorID * m_incrementSize };
}

uint32 GPUDescriptorHeap::GetDescriptorIDFromHandle(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	return static_cast<uint32>((handle.ptr - m_gpuStart[m_heapIndex].ptr) / m_incrementSize);
}

}