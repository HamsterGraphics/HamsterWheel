/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "Containers/Vector.h"
#include "IGraphics.h"

namespace hg
{

typedef struct GPUDescriptorAllocation
{
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;
} GPUDescriptorAllocation;

class GPUDescriptorHeap
{
public:
	GPUDescriptorHeap() = delete;
	explicit GPUDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 persistentDescriptorCount, uint32 tempDescriptorCount);
	GPUDescriptorHeap(const GPUDescriptorHeap&) = delete;
	GPUDescriptorHeap& operator=(const GPUDescriptorHeap&) = delete;
	GPUDescriptorHeap(GPUDescriptorHeap&&) = delete;
	GPUDescriptorHeap& operator=(GPUDescriptorHeap&&) = delete;
	~GPUDescriptorHeap() = default;

	D3D12_DESCRIPTOR_HEAP_TYPE GetHeapType() const { return m_type; }
	uint32 GetDescriptorCount() const { return m_persistentDescriptorCount + m_tempDescriptorCount; }

	void EndFrame();
	GPUDescriptorAllocation Allocate();
	GPUDescriptorAllocation AllocateTemp();
	void Free(GPUDescriptorAllocation handle);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleFromDescriptorID(uint32 descriptorID);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleFromDescriptorID(uint32 descriptorID);
	uint32 GetDescriptorIDFromHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle);
	uint32 GetDescriptorIDFromHandle(D3D12_GPU_DESCRIPTOR_HANDLE handle);

private:
	RefCountPtr<ID3D12DescriptorHeap> m_heap[MAX_FRAME_COUNT];
	D3D12_DESCRIPTOR_HEAP_TYPE m_type;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuStart[MAX_FRAME_COUNT];
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpuStart[MAX_FRAME_COUNT];
	uint32 m_incrementSize;
	uint32 m_persistentDescriptorCount;
	uint32 m_tempDescriptorCount;
	uint32 m_totalDescriptorCount;
	uint32 m_heapIndex;
	std::atomic<uint32> m_allocatedIndex;
	std::atomic<uint32> m_tempAllocatedIndex;
	Vector<uint32> m_descriptorIDs;
};

}