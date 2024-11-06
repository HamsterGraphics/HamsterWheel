/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "Fence.h"

#include "CommandQueue.h"

namespace hg
{

Fence::Fence(ID3D12Device* pDevice)
{
	D3D12_VERIFY(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_completeEvent = ::CreateEventExA(nullptr, "Fence Event", 0, EVENT_ALL_ACCESS);
	Assert(m_completeEvent != 0);
}

Fence::~Fence()
{
	::CloseHandle(m_completeEvent);;
}

void Fence::Signal(CommandQueue* pQueue, uint64 fenceValue)
{
	D3D12_VERIFY(pQueue->GetHandle()->Signal(m_fence, fenceValue));
}

void Fence::CPUWait(uint64 fenceValue)
{
	if (m_fence->GetCompletedValue() < fenceValue)
	{
		D3D12_VERIFY(m_fence->SetEventOnCompletion(fenceValue, m_completeEvent));
		::WaitForSingleObject(m_completeEvent, INFINITE);
	}
}

}