/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "IGraphics.h"

namespace hg
{

class GPUDescriptorHeap
{
public:
	GPUDescriptorHeap() = delete;
	explicit GPUDescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 descriptorCount);
	GPUDescriptorHeap(const GPUDescriptorHeap&) = delete;
	GPUDescriptorHeap& operator=(const GPUDescriptorHeap&) = delete;
	GPUDescriptorHeap(GPUDescriptorHeap&&) = delete;
	GPUDescriptorHeap& operator=(GPUDescriptorHeap&&) = delete;
	~GPUDescriptorHeap() = default;

private:
	RefCountPtr<ID3D12DescriptorHeap> m_heap;
	D3D12_DESCRIPTOR_HEAP_TYPE m_type;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuStart;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpuStart;
	uint32 m_incrementSize;
	uint32 m_descriptorCount;
};

}