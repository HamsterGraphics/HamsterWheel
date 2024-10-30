/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Containers/Vector.h"
#include "IGraphics.h"

#include <atomic>

namespace hg
{

class CPUDescriptorHeap
{
public:
	CPUDescriptorHeap() = delete;
	explicit CPUDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 descriptorCount);
	CPUDescriptorHeap(const CPUDescriptorHeap&) = delete;
	CPUDescriptorHeap& operator=(const CPUDescriptorHeap&) = delete;
	CPUDescriptorHeap(CPUDescriptorHeap&&) = delete;
	CPUDescriptorHeap& operator=(CPUDescriptorHeap&&) = delete;
	~CPUDescriptorHeap() = default;

	D3D12_CPU_DESCRIPTOR_HANDLE Allocate();
	void Free(D3D12_CPU_DESCRIPTOR_HANDLE handle);

private:
	RefCountPtr<ID3D12DescriptorHeap> m_heap;
	D3D12_DESCRIPTOR_HEAP_TYPE m_type;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuStart;
	uint32 m_incrementSize;
	uint32 m_descriptorCount;
	std::atomic<uint32> m_allocatedIndices;
	Vector<uint32> m_descriptorIDs;
};

}