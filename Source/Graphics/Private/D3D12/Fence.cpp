/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "Fence.h"

namespace hg
{

Fence::Fence(ID3D12Device* pDevice) :
	m_fenceValue(0)
{
	D3D12_VERIFY(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_completeEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

Fence::~Fence()
{
	::CloseHandle(m_completeEvent);;
}

}