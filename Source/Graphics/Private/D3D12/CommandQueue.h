/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#pragma once

#include "IGraphics.h"

#include <memory>

namespace hg
{

class Fence;

class CommandQueue
{
public:
	CommandQueue() = delete;
	explicit CommandQueue(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE queueType);
	CommandQueue(const CommandQueue&) = delete;
	CommandQueue& operator=(const CommandQueue&) = delete;
	CommandQueue(CommandQueue&&) = delete;
	CommandQueue& operator=(CommandQueue&&) = delete;
	~CommandQueue() = default;

	ID3D12CommandQueue* GetHandle() const { return m_queue; }

private:
	RefCountPtr<ID3D12CommandQueue> m_queue;
	std::unique_ptr<Fence> m_fence;
	D3D12_COMMAND_LIST_TYPE m_type;
};

}