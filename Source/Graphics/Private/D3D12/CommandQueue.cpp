/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include "CommandQueue.h"
#include "Fence.h"

namespace hg
{

CommandQueue::CommandQueue(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE queueType) :
	m_type(queueType)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.NodeMask = 0;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = queueType;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    D3D12_VERIFY(pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue)));

    m_fence = std::make_unique<Fence>(pDevice);
}

}