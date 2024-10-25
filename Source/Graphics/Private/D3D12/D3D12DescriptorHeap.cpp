/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "D3D12Context.h"

void DescriptorHeap_Init(DescriptorHeap* pDescriptorHeap, ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
{
	Mutex_Init(&pDescriptorHeap->HeapMutex);

	pDescriptorHeap->Device = pDevice;
	pDescriptorHeap->IncrementSize = pDevice->GetDescriptorHandleIncrementSize(desc.Type);
	
	D3D12_VERIFY(pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pDescriptorHeap->Heap)));
	pDescriptorHeap->CPUStart = pDescriptorHeap->Heap->GetCPUDescriptorHandleForHeapStart();
	if (desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
	{
		pDescriptorHeap->GPUStart = pDescriptorHeap->Heap->GetGPUDescriptorHandleForHeapStart();
	}
}