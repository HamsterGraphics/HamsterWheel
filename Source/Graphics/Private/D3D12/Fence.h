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

class CommandQueue;

class Fence
{
public:
	Fence() = delete;
	explicit Fence(ID3D12Device* pDevice);
	Fence(const Fence&) = delete;
	Fence& operator=(const Fence&) = delete;
	Fence(Fence&&) = delete;
	Fence& operator=(Fence&&) = delete;
	~Fence();

	void Signal(CommandQueue* pQueue, uint64 fenceValue);
	void CPUWait(uint64 fenceValue);

private:
	RefCountPtr<ID3D12Fence> m_fence;
	HANDLE m_completeEvent;
};

}