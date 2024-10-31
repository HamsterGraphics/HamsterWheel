/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "CPUDescriptorHeap.h"
#include "Math/Math.h"

namespace hg
{

CPUDescriptorHeap::CPUDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 descriptorCount) :
	m_type(heapType)
{
	m_descriptorCount = RoundUp(descriptorCount, 32);
	m_descriptorIDs.resize(m_descriptorCount);
	std::iota(m_descriptorIDs.begin(), m_descriptorIDs.end(), 0);
	m_incrementSize = pDevice->GetDescriptorHandleIncrementSize(m_type);

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = m_type;
	desc.NumDescriptors = m_descriptorCount;
	D3D12_VERIFY(pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap)));
	m_cpuStart = m_heap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHeap::Allocate()
{
	uint32 allocateIndex = m_allocatedIndex.fetch_add(1);
	Assert(allocateIndex < m_descriptorCount);
	uint32 descriptorID = m_descriptorIDs[allocateIndex];
	return GetHandleFromDescriptorID(descriptorID);
}

void CPUDescriptorHeap::Free(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
	uint32 descriptorID = GetDescriptorIDFromHandle(handle);
	uint32 freeIndex = m_allocatedIndex.fetch_sub(1) - 1;
	Assert(freeIndex >= 0);
	m_descriptorIDs[freeIndex] = descriptorID;
}

D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHeap::GetHandleFromDescriptorID(uint32 descriptorID)
{
	return D3D12_CPU_DESCRIPTOR_HANDLE { .ptr = m_cpuStart.ptr + descriptorID * m_incrementSize };
}

uint32 CPUDescriptorHeap::GetDescriptorIDFromHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
	return static_cast<uint32>((handle.ptr - m_cpuStart.ptr) / m_incrementSize);
}

}